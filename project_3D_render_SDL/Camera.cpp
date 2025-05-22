#pragma once
#include "SDL3/SDL.h"
#include <cmath>
#include <chrono>
#include <vector>
#include <string>
#include "imgui.h"
#include "Camera.h"

struct Point2D { float x, y; };
struct Point3D { float x, y, z; };
struct Vertex { int start, end; };

struct RenderableModel
{
    std::string name;
    std::vector<Point3D> points;
    std::vector<Vertex> vertices;
};

class Renderer3D
{
public:
    Renderer3D(SDL_Window* window, SDL_Renderer* renderer);

    void loadModel(const RenderableModel& model);
    void renderSceneContent();
    void updateRotation(float deltaTime);
    void resetView();
    void resetRotationAngles();

    ImVec4 modelColor;
    ImVec4 backgroundColor;

    bool autoRotate;
    bool rotateXEnabled;
    bool rotateYEnabled;
    bool rotateZEnabled;

    float rotationSpeedX;
    float rotationSpeedY;
    float rotationSpeedZ;

    int windowWidth = 0;
    int windowHeight = 0;

    Camera& getCamera() { return m_camera; }
    const Camera& getCamera() const { return m_camera; }

private:
    Point3D rotatePoint(Point3D point, float angleX, float angleY, float angleZ);
    Point2D projection(Point3D point);

    float currentRotationX;
    float currentRotationY;
    float currentRotationZ;

    SDL_Renderer* sdl_renderer_ptr;
    std::vector<Point3D> current_points;
    std::vector<Vertex> current_vertices;

    Camera m_camera;

    const ImVec4 DEFAULT_MODEL_COLOR = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    const ImVec4 DEFAULT_BACKGROUND_COLOR = ImVec4(0.1f, 0.1f, 0.12f, 1.0f); // 
};