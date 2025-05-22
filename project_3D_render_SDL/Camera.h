#pragma once
#include <cmath>

const float DEFAULT_FOV = 60.0f;
const float DEFAULT_CAMERA_DISTANCE = 3.0f;

class Camera
{
public:
    float fov;
    float cameraDistance;

    Camera()
        : fov(DEFAULT_FOV), cameraDistance(DEFAULT_CAMERA_DISTANCE) {
    }

    void reset() {
        fov = DEFAULT_FOV;
        cameraDistance = DEFAULT_CAMERA_DISTANCE;
    }

    // В будущем сюда можно добавить:
    // - Положение камеры (position)
    // - Точку, куда смотрит камера (target/lookAt)
    // - Вектор "вверх" (up_vector)
    // - Методы для перемещения и вращения камеры (pan, tilt, zoom, orbit)
    // - Методы для получения матриц вида (View Matrix) и проекции (Projection Matrix)
};