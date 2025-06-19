#pragma once
#include <cmath>
#include <algorithm>

#ifndef M_PI_F
#define M_PI_F 3.1415926535f
#endif // !M_PI_F


const float DEFAULT_FOV = 60.0f;
const float NORMALIZED_MODEL_DEFAULT_CAMERA_DISTANCE = 4.0f;

class Camera
{
public:
    float fov;
    float cameraDistance;

    Camera()
        : fov(DEFAULT_FOV), cameraDistance(NORMALIZED_MODEL_DEFAULT_CAMERA_DISTANCE) {
    }

    void reset() {
        fov = DEFAULT_FOV;
        cameraDistance = NORMALIZED_MODEL_DEFAULT_CAMERA_DISTANCE;
    }

};