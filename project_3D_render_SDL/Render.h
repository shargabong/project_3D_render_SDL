#pragma once
#include "SDL3/SDL.h"
#include <cmath>
#include <chrono>
#include <vector>
#include <string>
#include "imgui.h"
#include "Camera.h"
#include "MathUtils.h"

struct Edge { int start, end; };

struct RenderableModel
{
	std::string name;
	std::vector<Vec3> points;
	std::vector<Edge> edges;

	std::vector<Vec2> texcoords_pool;
	std::vector<Vec3> normals_pool;
	std::vector<ObjFace> faces_data;

	RenderableModel() = default;
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
	Vec3 rotatePoint(Vec3 point, float angleX, float angleY, float angleZ);
	Vec2 projection(Vec3 point);

	float currentRotationX;
	float currentRotationY;
	float currentRotationZ;

	SDL_Renderer* sdl_renderer_ptr;
	std::vector<Vec3> current_points_to_render;
	std::vector<Edge> current_edges_to_render;

	Camera m_camera;

	const ImVec4 DEFAULT_MODEL_COLOR = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	const ImVec4 DEFAULT_BACKGROUND_COLOR = ImVec4(0.1f, 0.1f, 0.12f, 1.0f);
};