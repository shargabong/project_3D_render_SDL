#pragma once
#include <vector>
#include <cmath>
#include <algorithm>

//#ifndef M_PI_F
//#define M_PI_F 3.1415926
//#endif // !M_PI_F


struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;
};

struct Vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vec3& operator-=(const Vec3& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }
    Vec3& operator*=(float scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }
};

struct FaceVertexIndex {
    int v_idx = -1;
    int vt_idx = -1;
    int vn_idx = -1;
};

struct ObjFace {
    std::vector<FaceVertexIndex> indices;
};