#include "Render.h"
#include <iostream>
#include <algorithm>

Renderer3D::Renderer3D(SDL_Window* _window, SDL_Renderer* _renderer)
    : sdl_renderer_ptr(_renderer),
    currentRotationX(0.0f), currentRotationY(0.0f), currentRotationZ(0.0f),
    autoRotate(true),
    rotateXEnabled(false), rotateYEnabled(true), rotateZEnabled(false), // По умолчанию вращаем по Y
    rotationSpeedX(0.5f), rotationSpeedY(0.5f), rotationSpeedZ(0.5f)
{
    SDL_GetWindowSize(_window, &windowWidth, &windowHeight);
    resetView(); // Устанавливаем значения по умолчанию
}

void Renderer3D::resetView() {
    fov = DEFAULT_FOV;
    cameraDistance = DEFAULT_CAMERA_DISTANCE;
    modelColor = DEFAULT_MODEL_COLOR;
    backgroundColor = DEFAULT_BACKGROUND_COLOR;
    currentRotationX = 0.0f;
    currentRotationY = 0.0f;
    currentRotationZ = 0.0f;
    // Можно сбросить и скорости/флаги вращения, если нужно
    // autoRotate = true;
    // rotateXEnabled = false; rotateYEnabled = true; rotateZEnabled = false;
}

void Renderer3D::resetRotationAngles() {
    currentRotationX = 0.0f;
    currentRotationY = 0.0f;
    currentRotationZ = 0.0f;
}

void Renderer3D::loadModel(const RenderableModel& model)
{
    current_points = model.points;
    current_vertices = model.vertices;
    // currentRotationX = 0.0f; // Сброс углов при загрузке модели
    // currentRotationY = 0.0f;
    // currentRotationZ = 0.0f;
    // Вместо этого лучше вызывать resetView() или часть его логики, если хотим полный сброс
    std::cout << "Loaded model: " << model.name << std::endl;
}

void Renderer3D::updateRotation(float deltaTime)
{
    if (autoRotate) {
        if (rotateXEnabled) currentRotationX += rotationSpeedX * deltaTime;
        if (rotateYEnabled) currentRotationY += rotationSpeedY * deltaTime;
        if (rotateZEnabled) currentRotationZ += rotationSpeedZ * deltaTime;

        // Ограничиваем углы, чтобы избежать переполнения float со временем
        auto wrap_angle = [](float angle) {
            return fmod(angle, 2.0f * SDL_PI_F);
            };
        currentRotationX = wrap_angle(currentRotationX);
        currentRotationY = wrap_angle(currentRotationY);
        currentRotationZ = wrap_angle(currentRotationZ);
    }
}

Point3D Renderer3D::rotatePoint(Point3D point, float angleX, float angleY, float angleZ) {
    Point3D p = point;
    Point3D temp;

    // Вращение вокруг X
    if (angleX != 0.0f) {
        float cosX = cos(angleX);
        float sinX = sin(angleX);
        temp.x = p.x;
        temp.y = p.y * cosX - p.z * sinX;
        temp.z = p.y * sinX + p.z * cosX;
        p = temp;
    }

    // Вращение вокруг Y
    if (angleX != 0.0f) {
        float cosY = cos(angleY);
        float sinY = sin(angleY);
        temp.x = p.x * cosY + p.z * sinY;
        temp.y = p.y;
        temp.z = -p.x * sinY + p.z * cosY;
        p = temp;
    }

    // Вращение вокруг Z
    if (angleX != 0.0f) {
        float cosZ = cos(angleZ);
        float sinZ = sin(angleZ);
        temp.x = p.x * cosZ - p.y * sinZ;
        temp.y = p.x * sinZ - p.z * sinZ;
        temp.z = p.z;
        p = temp;
    }
    return p;
}

Point2D Renderer3D::projection(Point3D point)
{
    // Используем FOV для определения масштаба проекции.
    // Чем меньше FOV, тем "ближе" камера (больше масштаб).
    // Формула: distance = 1 / tan(fov_radians / 2)
    // Для простоты, можно использовать fov как прямой делитель, но правильнее так:
    float fov_rad = fov * SDL_PI_F / 180.0f;
    float projection_plane_dist = 1.0f / tan(fov_rad / 2.0f);

    float z_eff = point.z + cameraDistance;
    // Добавим небольшое значение к знаменателю, чтобы избежать деления на ноль или очень маленькое число
    // если объект слишком близко или за камерой.
    // Также, объекты за плоскостью проекции (z_eff <= 0) не должны рисоваться.
    // Но простой клиппинг здесь может быть сложен для линий.
    // Пока просто используем max для предотвращения деления на ноль/отрицательное.

    float perspective_factor = 0.0f;
    if (z_eff > 0.01f) { // Небольшой порог перед камерой
        perspective_factor = projection_plane_dist / z_eff;
    }

    float projection_x = point.x * perspective_factor;
    float projection_y = point.y * perspective_factor;

    // Масштабирование и центрирование
    // Масштаб подбирается так, чтобы объект размером 1x1 на расстоянии cameraDistance=DEFAULT_CAMERA_DISTANCE
    // занимал разумную часть экрана.
    float scale = std::min(windowHeight, windowHeight) / 2.5f; // Эмпирический подбор масштаба

    return Point2D{
        windowWidth / 2.0f + projection_x * scale,
        windowHeight / 2.0f - projection_y * scale // Y инвертирован в экранных координатах
    };
}

void Renderer3D::renderSceneContent()
{
    if (current_points.empty() || current_vertices.empty())
    {
        return;
    }
    
    // цвета модели
    SDL_SetRenderDrawColor(sdl_renderer_ptr,
        static_cast<Uint8>(modelColor.x * 255),
        static_cast<Uint8>(modelColor.y * 255),
        static_cast<Uint8>(modelColor.z * 255),
        static_cast<Uint8>(modelColor.w * 255));

    for (const auto& vertex : current_vertices)
    {
        if (vertex.start < 0 || vertex.start >= current_points.size() ||
            vertex.end < 0 || vertex.end >= current_points.size())
        {
            std::cerr << "Error: Vertex index out of bounds!" << std::endl;
            continue;
        }

        Point3D p_start = current_points[vertex.start];
        Point3D p_end = current_points[vertex.end];

        Point3D rotatedStartPoint = rotatePoint(p_start, currentRotationX, currentRotationY, currentRotationZ);
        Point3D rotatedEndPoint = rotatePoint(p_end, currentRotationX, currentRotationY, currentRotationZ);

        Point2D start_2d = projection(rotatedStartPoint);
        Point2D end_2d = projection(rotatedEndPoint);

        SDL_RenderLine(sdl_renderer_ptr, start_2d.x, start_2d.y, end_2d.x, end_2d.y);
    }
}