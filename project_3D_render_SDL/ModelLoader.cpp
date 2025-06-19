#include "ModelLoader.h" 
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> 
#include <vector>    
#include <cmath>     

std::vector<std::string> splitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

FaceVertexIndex parseFaceVertexToken(const std::string& token) {
    FaceVertexIndex fvi;
    std::vector<std::string> parts = splitString(token, '/');
    if (parts.size() >= 1 && !parts[0].empty()) {
        try { fvi.v_idx = std::stoi(parts[0]); }
        catch (...) { fvi.v_idx = -1; }
    }
    if (parts.size() >= 2 && !parts[1].empty()) {
        try { fvi.vt_idx = std::stoi(parts[1]); }
        catch (...) { fvi.vt_idx = -1; }
    }
    if (parts.size() >= 3 && !parts[2].empty()) {
        try { fvi.vn_idx = std::stoi(parts[2]); }
        catch (...) { fvi.vn_idx = -1; }
    }
    return fvi;
}

namespace ModelLoader {

    bool loadObj(const std::string& filepath, RenderableModel& model) {
        model.points.clear();
        model.edges.clear();
        model.texcoords_pool.clear();
        model.normals_pool.clear();
        model.faces_data.clear();
        model.name = "";

        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open OBJ file: " << filepath << std::endl;
            return false;
        }

        std::string line;
        std::vector<Vec3> temp_points;
        std::vector<Vec2> temp_texcoords;
        std::vector<Vec3> temp_normals;
        std::vector<ObjFace> temp_faces;

        while (std::getline(file, line)) {
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
            if (line.empty() || line[0] == '#') continue;

            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") {
                Vec3 point;
                if (!(iss >> point.x >> point.y >> point.z)) { continue; }
                temp_points.push_back(point);
            }
            else if (prefix == "vt") {
                Vec2 texcoord;
                if (!(iss >> texcoord.x >> texcoord.y)) { continue; }
                temp_texcoords.push_back(texcoord);
            }
            else if (prefix == "vn") {
                Vec3 normal;
                if (!(iss >> normal.x >> normal.y >> normal.z)) { continue; }
                temp_normals.push_back(normal);
            }
            else if (prefix == "f") {
                ObjFace current_face;
                std::string token;
                while (iss >> token) {
                    current_face.indices.push_back(parseFaceVertexToken(token));
                }
                if (!current_face.indices.empty()) temp_faces.push_back(current_face);
            }
            else if ((prefix == "o" || prefix == "g") && model.name.empty()) {
                iss >> model.name;
            }
        }
        file.close();

        if (model.name.empty()) {
            size_t last_slash = filepath.find_last_of("/\\");
            model.name = (last_slash == std::string::npos) ? filepath : filepath.substr(last_slash + 1);
            size_t dot_pos = model.name.find_last_of('.');
            if (dot_pos != std::string::npos) {
                model.name = model.name.substr(0, dot_pos);
            }
        }

        model.points = temp_points;
        model.texcoords_pool = temp_texcoords;
        model.normals_pool = temp_normals;
        model.faces_data = temp_faces;

        if (!model.points.empty()) {
            Vec3 minP = model.points[0];
            Vec3 maxP = model.points[0];

            for (size_t i = 1; i < model.points.size(); ++i) {
                const auto& p = model.points[i];
                minP.x = std::min(minP.x, p.x); minP.y = std::min(minP.y, p.y); minP.z = std::min(minP.z, p.z);
                maxP.x = std::max(maxP.x, p.x); maxP.y = std::max(maxP.y, p.y); maxP.z = std::max(maxP.z, p.z);
            }

            Vec3 center;
            center.x = (minP.x + maxP.x) / 2.0f;
            center.y = (minP.y + maxP.y) / 2.0f;
            center.z = (minP.z + maxP.z) / 2.0f;

            float modelWidth = maxP.x - minP.x;
            float modelHeight = maxP.y - minP.y;
            float modelDepth = maxP.z - minP.z;

            float maxDim = 0.0f;
            if (modelWidth > maxDim) maxDim = modelWidth;
            if (modelHeight > maxDim) maxDim = modelHeight;
            if (modelDepth > maxDim) maxDim = modelDepth;

            float desiredSize = 2.0f;
            float scaleFactor = 1.0f;
            if (maxDim > 1e-6f) {
                scaleFactor = desiredSize / maxDim;
            }

            for (auto& p : model.points) {
                p.x -= center.x;    p.y -= center.y;    p.z -= center.z;
                p.x *= scaleFactor; p.y *= scaleFactor; p.z *= scaleFactor;
            }

            std::cout << "Model normalized: center offset (" << center.x << "," << center.y << "," << center.z
                << "), original maxDim: " << maxDim << ", scaleFactor applied: " << scaleFactor << std::endl;
        }

        std::vector<std::pair<int, int>> unique_edge_pairs;
        for (const auto& face : model.faces_data) {
            if (face.indices.size() < 2) continue;
            for (size_t i = 0; i < face.indices.size(); ++i) {
                int idx1_0based = face.indices[i].v_idx - 1;
                int idx2_0based = face.indices[(i + 1) % face.indices.size()].v_idx - 1;
                if (idx1_0based < 0 || idx1_0based >= static_cast<int>(model.points.size()) ||
                    idx2_0based < 0 || idx2_0based >= static_cast<int>(model.points.size())) {
                    continue;
                }
                std::pair<int, int> edge_pair = (idx1_0based < idx2_0based) ?
                    std::make_pair(idx1_0based, idx2_0based) :
                    std::make_pair(idx2_0based, idx1_0based);
                bool found = false;
                for (const auto& ue : unique_edge_pairs) {
                    if (ue.first == edge_pair.first && ue.second == edge_pair.second) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    model.edges.push_back({ idx1_0based, idx2_0based });
                    unique_edge_pairs.push_back(edge_pair);
                }
            }
        }

        std::cout << "Loaded OBJ model: " << model.name
            << " (Vertices: " << model.points.size()
            << ", Edges for wireframe: " << model.edges.size()
            << ")" << std::endl;
        return true;
    }

}