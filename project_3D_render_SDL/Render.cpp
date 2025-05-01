#include "Render.h"

Renderer3D::Renderer3D(SDL_Window* _window, SDL_Renderer* _renderer, std::vector<Point3D>& _points, std::vector<Vertex>& _vertices) {
    // Получаем размеры окна
    SDL_GetWindowSize(_window, &WindowSizeX, &WindowSizeY);
    renderer = _renderer;
    points = _points;
    vertices = _vertices;
}

void Renderer3D::render() {
    // Измеряем время для независимого от кадров тайминга
    auto time1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration(0);

    // Устанавливаем цвет рендеринга в черный и очищаем рендерер
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // Устанавливаем цвет рендеринга в белый
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    // Обновляем угол вращения
    rotation += 1 * DeltaTime;

    // Перебираем все вершины
    for (auto& vertex : vertices) {
        // Вращаем начальную точку вершины вокруг осей X и Y
        Point3D rotatedStartPoint = (rotateX(rotateY(points[vertex.start])));

        // Вращаем конечную точку вершины вокруг осей X и Y
        Point3D rotatedEndPoint = (rotateX(rotateY(points[vertex.end])));

        // Проецируем точки из 3D в 2D
        Point2D start = projection(rotatedStartPoint);
        Point2D end = projection(rotatedEndPoint);

        // Рисуем линию между точками
        SDL_RenderLine(renderer, start.x, start.y, end.x, end.y);
    }

    // Обновляем экран
    SDL_RenderPresent(renderer);

    // Измеряем время после рендеринга
    auto time2 = std::chrono::high_resolution_clock::now();
    duration = time2 - time1;
    DeltaTime = duration.count();
    time1 = time2;
}

Point3D Renderer3D::rotateX(Point3D point) {
    // Вращаем точку вокруг оси X
    Point3D returnPoint;
    returnPoint.x = point.x;
    returnPoint.y = cos(rotation) * point.y - sin(rotation) * point.z;
    returnPoint.z = sin(rotation) * point.y + cos(rotation) * point.z;
    return returnPoint;
}

Point3D Renderer3D::rotateY(Point3D point) {
    // Вращаем точку вокруг оси Y
    Point3D returnPoint;
    returnPoint.x = cos(rotation) * point.x - sin(rotation) * point.z;
    returnPoint.y = point.y;
    returnPoint.z = sin(rotation) * point.x + cos(rotation) * point.z;
    return returnPoint;
}

Point2D Renderer3D::projection(Point3D point) {
    // Проецируем точку из 3D в 2D
    return Point2D{
        WindowSizeX / 2 + (FOV * point.x) / (FOV + point.z) * 100,
        WindowSizeY / 2 + (FOV * point.y) / (FOV + point.z) * 100
    };
}