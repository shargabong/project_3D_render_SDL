#include "ModelLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

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
        catch (const std::invalid_argument& ia) { std::cerr << "Invalid argument for v_idx: " << parts[0] << std::endl; fvi.v_idx = -1; }
        catch (const std::out_of_range& oor) { std::cerr << "Out of range for v_idx: " << parts[0] << std::endl; fvi.v_idx = -1; }
    }
    if (parts.size() >= 2 && !parts[1].empty()) {
        try { fvi.vt_idx = std::stoi(parts[1]); }
        catch (const std::invalid_argument& ia) { std::cerr << "Invalid argument for vt_idx: " << parts[1] << std::endl; fvi.vt_idx = -1; }
        catch (const std::out_of_range& oor) { std::cerr << "Out of range for vt_idx: " << parts[1] << std::endl; fvi.vt_idx = -1; }
    }
    if (parts.size() >= 3 && !parts[2].empty()) {
        try { fvi.vn_idx = std::stoi(parts[2]); }
        catch (const std::invalid_argument& ia) { std::cerr << "Invalid argument for vn_idx: " << parts[2] << std::endl; fvi.vn_idx = -1; }
        catch (const std::out_of_range& oor) { std::cerr << "Out of range for vn_idx: " << parts[2] << std::endl; fvi.vn_idx = -1; }
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

            if (line.empty() || line[0] == '#') {
                continue;
            }

            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") {
                Vec3 point;
                if (!(iss >> point.x >> point.y >> point.z)) {
                    std::cerr << "Warning: Malformed vertex line in OBJ: " << line << std::endl;
                    continue;
                }
                temp_points.push_back(point);
            }
            else if (prefix == "vt") {
                Vec2 texcoord;
                if (!(iss >> texcoord.x >> texcoord.y)) {
                    std::cerr << "Warning: Malformed texture coordinate line in OBJ: " << line << std::endl;
                    continue;
                }
                temp_texcoords.push_back(texcoord);
            }
            else if (prefix == "vn") {
                Vec3 normal;
                if (!(iss >> normal.x >> normal.y >> normal.z)) {
                    std::cerr << "Warning: Malformed normal line in OBJ: " << line << std::endl;
                    continue;
                }
                temp_normals.push_back(normal);
            }
            else if (prefix == "f") {
                ObjFace current_face;
                std::string token;
                while (iss >> token) {
                    current_face.indices.push_back(parseFaceVertexToken(token));
                }
                if (!current_face.indices.empty()) {
                    temp_faces.push_back(current_face);
                }
                else {
                    std::cerr << "Warning: Empty face definition in OBJ: " << line << std::endl;
                }
            }
            else if (prefix == "o" || prefix == "g") {
                if (model.name.empty()) {
                    iss >> model.name;
                }
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

        std::vector<std::pair<int, int>> unique_edge_pairs;

        for (const auto& face : model.faces_data) {
            if (face.indices.size() < 2) continue;

            for (size_t i = 0; i < face.indices.size(); ++i) {
                int idx1_0based = face.indices[i].v_idx - 1;
                int idx2_0based = face.indices[(i + 1) % face.indices.size()].v_idx - 1;

                if (idx1_0based < 0 || idx1_0based >= static_cast<int>(model.points.size()) ||
                    idx2_0based < 0 || idx2_0based >= static_cast<int>(model.points.size())) {
                    std::cerr << "Warning: Invalid vertex index (" << face.indices[i].v_idx << " or "
                        << face.indices[(i + 1) % face.indices.size()].v_idx
                        << ") in face for model " << model.name << ". Skipping edge." << std::endl;
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
            << ", TexCoords: " << model.texcoords_pool.size()
            << ", Normals: " << model.normals_pool.size()
            << ", Faces read: " << model.faces_data.size()
            << ", Edges for wireframe: " << model.edges.size()
            << ")" << std::endl;

        return true;
    }

}