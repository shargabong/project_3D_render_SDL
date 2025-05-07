#pragma once
#include "SDL3/SDL.h"
#include <cmath>
#include <chrono>
#include <vector>
#include <string>

struct Point2D { float x, y; };
struct Point3D { float x, y, z; };
struct Vertex { int start, end; }; // вершина - пространство между точками

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

	void render();

private:
	Point3D rotateX(Point3D point);
	Point3D rotateY(Point3D point);
	Point2D projection(Point3D point);

	float rotation = 0.0f;
	float FOV = 100.0f;
	float DeltaTime = 0.0f;

	int WindowSizeX = 0;
	int WindowSizeY = 0;
	SDL_Renderer* sdl_renderer_ptr;

	std::vector<Point3D> current_points;
	std::vector<Vertex> current_vertices;

	std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime;
};

//(олд версия)
//class Renderer3D {
//public:
//	Renderer3D(SDL_Window* window, SDL_Renderer* renderer, std::vector<Point3D>& points, std::vector<Vertex>& vertices);
//	void render();
//private:
//	Point3D rotateX(Point3D point);
//	Point3D rotateY(Point3D point);
//	Point2D projection(Point3D point);
//
//	float rotation = 0.0f;
//	float FOV = 100.0f;
//	float DeltaTime = 0.0f;
//
//	int WindowSizeX;
//	int WindowSizeY;
//	SDL_Renderer* renderer;
//
//	std::vector<Point3D> points;
//	std::vector<Vertex> vertices;
//
//	std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime;
//};