#include "Render.h"
#include <iostream>
#include <algorithm> 

#ifndef M_PI_F
#define M_PI_F 3.1415926535f
#endif

Renderer3D::Renderer3D(SDL_Window* _window, SDL_Renderer* _renderer)
    : sdl_renderer_ptr(_renderer),
    modelRotationX(0.0f), modelRotationY(0.0f), modelRotationZ(0.0f),
    autoRotateModel(true),
    rotateModelXEnabled(false), rotateModelYEnabled(true), rotateModelZEnabled(false),
    modelRotationSpeedX(0.5f), modelRotationSpeedY(0.5f), modelRotationSpeedZ(0.5f)
{
    SDL_GetWindowSize(_window, &windowWidth, &windowHeight);
    resetView();
}

void Renderer3D::resetView() {
    m_camera.reset();
    modelColor = DEFAULT_MODEL_COLOR;
    backgroundColor = DEFAULT_BACKGROUND_COLOR;
    modelRotationX = 0.0f;
    modelRotationY = 0.0f;
    modelRotationZ = 0.0f;
}

void Renderer3D::resetRotationAngles() {
    modelRotationX = 0.0f;
    modelRotationY = 0.0f;
    modelRotationZ = 0.0f;
}

void Renderer3D::loadModel(const RenderableModel& model)
{
    current_points_to_render = model.points;
    current_edges_to_render = model.edges;
    std::cout << "Renderer processing model: " << model.name
        << " (Points: " << current_points_to_render.size()
        << ", Edges: " << current_edges_to_render.size() << ")" << std::endl;

    m_camera.reset();
    resetRotationAngles();
}

void Renderer3D::updateRotation(float deltaTime)
{
    if (autoRotateModel) {
        if (rotateModelXEnabled) modelRotationX += modelRotationSpeedX * deltaTime;
        if (rotateModelYEnabled) modelRotationY += modelRotationSpeedY * deltaTime;
        if (rotateModelZEnabled) modelRotationZ += modelRotationSpeedZ * deltaTime;

        auto wrap_angle = [](float angle) {
            return fmod(angle, 2.0f * M_PI_F);
            };
        modelRotationX = wrap_angle(modelRotationX);
        modelRotationY = wrap_angle(modelRotationY);
        modelRotationZ = wrap_angle(modelRotationZ);
    }
}

Vec3 Renderer3D::rotatePoint(Vec3 point, float angleX, float angleY, float angleZ) {
    Vec3 p = point;
    Vec3 temp;
    if (angleX != 0.0f) {
        float cosX = cos(angleX); float sinX = sin(angleX);
        temp.x = p.x; temp.y = p.y * cosX - p.z * sinX; temp.z = p.y * sinX + p.z * cosX;
        p = temp;
    }
    if (angleY != 0.0f) {
        float cosY = cos(angleY); float sinY = sin(angleY);
        temp.x = p.x * cosY + p.z * sinY; temp.y = p.y; temp.z = -p.x * sinY + p.z * cosY;
        p = temp;
    }
    if (angleZ != 0.0f) {
        float cosZ = cos(angleZ); float sinZ = sin(angleZ);
        temp.x = p.x * cosZ - p.y * sinZ; temp.y = p.x * sinZ + p.y * cosZ; temp.z = p.z;
        p = temp;
    }
    return p;
}

Vec2 Renderer3D::projection(Vec3 point)
{
    float fov_rad = m_camera.fov * M_PI_F / 180.0f;
    Vec3 p_camera_space = point;
    p_camera_space.z += m_camera.cameraDistance;

    float projected_x = 0.0f;
    float projected_y = 0.0f;
    float n = 0.1f;

    if (p_camera_space.z > n) {
        float tan_half_fovy = tan(fov_rad / 2.0f);
        if (std::abs(tan_half_fovy) < 1e-6f) tan_half_fovy = (tan_half_fovy > 0 ? 1e-6f : -1e-6f);
        float projection_plane_dist = 1.0f / tan_half_fovy;
        projected_x = p_camera_space.x * (projection_plane_dist / p_camera_space.z);
        projected_y = p_camera_space.y * (projection_plane_dist / p_camera_space.z);
    }
    else {
        projected_x = (p_camera_space.x > 0) ? 100000.0f : -100000.0f;
        projected_y = (p_camera_space.y > 0) ? 100000.0f : -100000.0f;
    }

    float scale_factor = std::min(static_cast<float>(windowWidth), static_cast<float>(windowHeight)) / 2.5f;

    return Vec2{
        windowWidth / 2.0f + projected_x * scale_factor,
        windowHeight / 2.0f - projected_y * scale_factor
    };
}

void Renderer3D::renderSceneContent()
{
    if (current_points_to_render.empty() || current_edges_to_render.empty())
    {
        return;
    }

    SDL_SetRenderDrawColor(sdl_renderer_ptr,
        static_cast<Uint8>(modelColor.x * 255),
        static_cast<Uint8>(modelColor.y * 255),
        static_cast<Uint8>(modelColor.z * 255),
        static_cast<Uint8>(modelColor.w * 255));

    for (const auto& edge : current_edges_to_render)
    {
        if (edge.start < 0 || static_cast<size_t>(edge.start) >= current_points_to_render.size() ||
            edge.end < 0 || static_cast<size_t>(edge.end) >= current_points_to_render.size())
        {
            continue;
        }

        Vec3 p_start_local = current_points_to_render[edge.start];
        Vec3 p_end_local = current_points_to_render[edge.end];

        Vec3 rotatedStartPoint = rotatePoint(p_start_local, modelRotationX, modelRotationY, modelRotationZ);
        Vec3 rotatedEndPoint = rotatePoint(p_end_local, modelRotationX, modelRotationY, modelRotationZ);

        Vec2 start_2d = projection(rotatedStartPoint);
        Vec2 end_2d = projection(rotatedEndPoint);

        SDL_RenderLine(sdl_renderer_ptr, start_2d.x, start_2d.y, end_2d.x, end_2d.y);
    }
}