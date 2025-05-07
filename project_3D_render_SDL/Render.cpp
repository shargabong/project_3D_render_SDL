#include "Render.h"
#include <iostream>

Renderer3D::Renderer3D(SDL_Window* _window, SDL_Renderer* _renderer)
{
    SDL_GetWindowSize(_window, &WindowSizeX, &WindowSizeY);
    sdl_renderer_ptr = _renderer;
    lastFrameTime = std::chrono::high_resolution_clock::now();
    rotation = 0.0f;
}

void Renderer3D::loadModel(const RenderableModel& model)
{
    current_points = model.points;
    current_vertices = model.vertices;
    rotation = 0.0f;
    std::cout << "Loaded model: " << model.name << std::endl;
}

void Renderer3D::render()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = currentTime - lastFrameTime;
    DeltaTime = duration.count();
    lastFrameTime = currentTime;

    SDL_SetRenderDrawColor(sdl_renderer_ptr, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(sdl_renderer_ptr);

    if (current_points.empty() || current_vertices.empty())
    {
        SDL_RenderPresent(sdl_renderer_ptr);
        return;
    }

    SDL_SetRenderDrawColor(sdl_renderer_ptr, 255, 255, 255, SDL_ALPHA_OPAQUE);

    rotation += 1.0f * DeltaTime;

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

        Point3D rotatedStartPoint = rotateX(rotateY(p_start));
        Point3D rotatedEndPoint = rotateX(rotateY(p_end));

        Point2D start_2d = projection(rotatedStartPoint);
        Point2D end_2d = projection(rotatedEndPoint);

        SDL_RenderLine(sdl_renderer_ptr, start_2d.x, start_2d.y, end_2d.x, end_2d.y);
    }

    SDL_RenderPresent(sdl_renderer_ptr);

}

Point3D Renderer3D::rotateX(Point3D point)
{
    Point3D returnPoint;
    float cos_r = cos(rotation);
    float sin_r = sin(rotation);
    returnPoint.x = point.x;
    returnPoint.y = cos_r * point.y - sin_r * point.z;
    returnPoint.z = sin_r * point.y + cos_r * point.z;
    return returnPoint;
}

Point3D Renderer3D::rotateY(Point3D point)
{
    Point3D returnPoint;
    float cos_r = cos(rotation);
    float sin_r = sin(rotation);
    returnPoint.x = cos_r * point.x - sin_r * point.z;
    returnPoint.y = point.y;
    returnPoint.z = sin_r * point.x + cos_r * point.z;
    return returnPoint;
}

Point2D Renderer3D::projection(Point3D point)
{
    Point3D returnPoint;
    float z_offset = 5.0f;
    float projected_x = point.x;
    float projected_y = point.y;

    if ((FOV + point.z + z_offset) > 0.001f)
    {
        projected_x = (FOV * point.x) / (FOV + point.z + z_offset);
        projected_y = (FOV * point.y) / (FOV + point.z + z_offset);
    }
    else {

    }


    float scale = 100.f;
    return Point2D
    {
        WindowSizeX / 2.0f + projected_x * scale,
        WindowSizeY / 2.0f - projected_y * scale
    };
}

//#include "Render.h"
//
//Renderer3D::Renderer3D(SDL_Window* _window, SDL_Renderer* _renderer, std::vector<Point3D>& _points, std::vector<Vertex>& _vertices) {
//    // Получаем размеры окна
//    SDL_GetWindowSize(_window, &WindowSizeX, &WindowSizeY);
//    renderer = _renderer;
//    points = _points;
//    vertices = _vertices;
//}
//
//void Renderer3D::render() {
//    // Измеряем время для независимого от кадров тайминга
//    auto time1 = std::chrono::high_resolution_clock::now();
//    std::chrono::duration<double> duration(0);
//
//    // Устанавливаем цвет рендеринга в черный и очищаем рендерер
//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
//    SDL_RenderClear(renderer);
//
//    // Устанавливаем цвет рендеринга в белый
//    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
//
//    // Обновляем угол вращения
//    rotation += 1.5 * DeltaTime;
//
//    // Перебираем все вершины
//    for (auto& vertex : vertices) {
//        // Вращаем начальную точку вершины вокруг осей X и Y
//        Point3D rotatedStartPoint = (rotateX(rotateY(points[vertex.start])));
//
//        // Вращаем конечную точку вершины вокруг осей X и Y
//        Point3D rotatedEndPoint = (rotateX(rotateY(points[vertex.end])));
//
//        // Проецируем точки из 3D в 2D
//        Point2D start = projection(rotatedStartPoint);
//        Point2D end = projection(rotatedEndPoint);
//
//        // Рисуем линию между точками
//        SDL_RenderLine(renderer, start.x, start.y, end.x, end.y);
//    }
//
//    // Обновляем экран
//    SDL_RenderPresent(renderer);
//
//    // Измеряем время после рендеринга
//    auto time2 = std::chrono::high_resolution_clock::now();
//    duration = time2 - time1;
//    DeltaTime = duration.count();
//    time1 = time2;
//}
//
//Point3D Renderer3D::rotateX(Point3D point) {
//    // Вращаем точку вокруг оси X
//    Point3D returnPoint;
//    returnPoint.x = point.x;
//    returnPoint.y = cos(rotation) * point.y - sin(rotation) * point.z;
//    returnPoint.z = sin(rotation) * point.y + cos(rotation) * point.z;
//    return returnPoint;
//}
//
//Point3D Renderer3D::rotateY(Point3D point) {
//    // Вращаем точку вокруг оси Y
//    Point3D returnPoint;
//    returnPoint.x = cos(rotation) * point.x - sin(rotation) * point.z;
//    returnPoint.y = point.y;
//    returnPoint.z = sin(rotation) * point.x + cos(rotation) * point.z;
//    return returnPoint;
//}
//
//Point2D Renderer3D::projection(Point3D point) {
//    // Проецируем точку из 3D в 2D
//    return Point2D{
//        WindowSizeX / 2 + (FOV * point.x) / (FOV + point.z) * 100,
//        WindowSizeY / 2 + (FOV * point.y) / (FOV + point.z) * 100
//    };
//}

//#include "Render.h"
//#include <iostream>
//
//Renderer3D::Renderer3D(SDL_Window* _window, SDL_Renderer* _renderer)
//{
//	SDL_GetWindowSize(_window, &WindowSizeX, &WindowSizeY);
//	sdl_renderer_ptr = _renderer;
//	lastFrameTime = std::chrono::high_resolution_clock::now();
//	rotation = 0.0f;
//}
//
//void Renderer3D::loadModel(const RenderableModel& model)
//{
//	current_points = model.points;
//	current_vertices = model.vertices;
//	rotation = 0.0f;
//	std::cout << "Loaded model:" << model.name << std::endl;
//}
//
//void Renderer3D::render()
//{
//	if (current_points.empty() || current_vertices.empty())
//	{
//		SDL_RenderClear(sdl_renderer_ptr);
//		SDL_RenderPresent(sdl_renderer_ptr);
//
//		auto currentTime = std::chrono::high_resolution_clock::now();
//		std::chrono::duration<double>duration = currentTime - lastFrameTime;
//		DeltaTime = duration.count();
//		lastFrameTime = currentTime;
//		return;
//	}
//
//	auto time1 = std::chrono::high_resolution_clock::now();
//
//	SDL_SetRenderDrawColor(sdl_renderer_ptr, 0, 0, 0, SDL_ALPHA_OPAQUE);
//	SDL_RenderClear(sdl_renderer_ptr);
//
//	SDL_SetRenderDrawColor(sdl_renderer_ptr, 255, 255, 255, SDL_ALPHA_OPAQUE);
//
//	rotation += 1.0f * DeltaTime;
//
//	for (const auto& vertex : current_vertices)
//	{
//		if (vertex.start < 0 || vertex.start >= current_points.size() || vertex.end < 0 || vertex.end >= current_points.size())
//		{
//			std::cerr << "Error: Vertex index out of bounds!" << std::endl;
//			continue;
//		}
//
//		Point3D p_start = current_points[vertex.start];
//		Point3D p_end = current_points[vertex.end];
//
//		Point3D rotatedStartPoint = rotateX(rotateY(p_start));
//		Point3D rotatedEndPoint = rotateX(rotateY(p_end));
//
//		Point2D start_2d = projection(rotatedStartPoint);
//		Point2D end_2d = projection(rotatedEndPoint);
//
//		SDL_RenderLine(sdl_renderer_ptr, start_2d.x, start_2d.y, end_2d.x, end_2d.y);
//	}
//
//	SDL_RenderPresent(sdl_renderer_ptr);
//
//	auto time2 = std::chrono::high_resolution_clock::now();
//	std::chrono::duration<double>duration = time2 - lastFrameTime;
//	DeltaTime = duration.count();
//	lastFrameTime = time2;
//}
//
//Point3D Renderer3D::rotateX(Point3D point)
//{
//	Point3D returnPoint;
//	float cos_r = cos(rotation);
//	float sin_r = sin(rotation);
//	returnPoint.x = point.x;
//	returnPoint.y = cos_r * point.y - sin_r * point.z;
//	returnPoint.z = sin_r * point.y + cos_r * point.z;
//	return returnPoint;
//}
//
//Point3D Renderer3D::rotateY(Point3D point)
//{
//	Point3D returnPoint;
//	float cos_r = cos(rotation);
//	float sin_r = sin(rotation);
//	returnPoint.x = cos_r * point.x - sin_r * point.z;
//	returnPoint.y = point.y;
//	returnPoint.z = sin_r * point.x + cos_r * point.z;
//	return returnPoint;
//}
//
//Point2D Renderer3D::projection(Point3D point)
//{
//	Point3D returnPoint;
//	float z_offset = 5.0f;
//	float projected_x = point.x;
//	float projected_y = point.y;
//	if ((FOV + point.z + z_offset) != 0)
//	{
//		projected_x = (FOV * point.x) / (FOV + point.z + z_offset);
//		projected_y = (FOV * point.y) / (FOV + point.z + z_offset);
//	}
//
//	float scale = 100.f;
//	return Point2D
//	{
//		WindowSizeX / 2.0f + projected_x * scale,
//		WindowSizeY / 2.0f - projected_y * scale
//	};
//}
