#include <SDL3/SDL.h>
#include <vector>
#include "Render.h"

#define SDL_RENDERER_ACCELERATED 0x00000002

//line 
//std::vector<Point3D> points{ 
//	Point3D{ -1.0f, -1.0f, -1.0f }, 
//	Point3D{ -1.0f, -1.0f, 1.0f }, 
//};
//
//std::vector<Vertex> vertices{ 
//	Vertex{ 0, 1 },
//};

//cube
std::vector<Point3D> points{
	Point3D{ -1.0f, -1.0f, -1.0f },
	Point3D{ -1.0f, -1.0f, 1.0f },
	Point3D{ 1.0f, -1.0f, -1.0f },
	Point3D{ -1.0f, 1.0f, -1.0f },
	Point3D{ -1.0f, 1.0f, 1.0f },
	Point3D{ 1.0f, -1.0f, 1.0f },
	Point3D{ 1.0f, 1.0f, -1.0f },
	Point3D{ 1.0f, 1.0f, 1.0f }
};

std::vector<Vertex> vertices{
	Vertex{ 0, 1 },
	Vertex{ 0, 2 },
	Vertex{ 0, 3 },
	Vertex{ 2, 5 },
	Vertex{ 3, 6 },
	Vertex{ 3, 4 },
	Vertex{ 4, 7 },
	Vertex{ 6, 7 },
	Vertex{ 7, 5 },
	Vertex{ 5, 1 },
	Vertex{ 4, 1 },
	Vertex{ 2, 6 },
};

//diamond
//std::vector<Point3D> points{
//    Point3D{ 0.0f, -1.0f, 0.0f },
//    Point3D{ -1.0f, 0.5f, 0.0f },
//	Point3D{ 1.0f, 0.5f, 0.0f },
//    Point3D{ -0.7f, 0.8f, 0.0f },
//    Point3D{ 0.7f, 0.8f, 0.0f },
//};
//
//std::vector<Vertex> vertices{
//    Vertex{ 0, 1 },
//    Vertex{ 0, 2 },
//    Vertex{ 1, 3 },
//    Vertex{ 2, 4 },
//    Vertex{ 3, 4 },
//};

int main(int argc, char* argv[]) {

	SDL_Window* window;
	SDL_Renderer* renderer;
	window = SDL_CreateWindow("3D Renderer", 800, 600, 0);
	renderer = SDL_CreateRenderer(window, 0);

	bool running = true;

	Renderer3D renderer3D1(window, renderer, points, vertices);

	while (running) {
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
				break;
			}
		}
		renderer3D1.render();
	}

	return 0;
}