#pragma once
#include <vector>

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;
};

struct Vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct FaceVertexIndex {
    int v_idx = -1;
    int vt_idx = -1;
    int vn_idx = -1;
};

struct ObjFace {
    std::vector<FaceVertexIndex> indices;
};