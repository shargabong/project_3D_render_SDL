#include "ModelFactory.h"

namespace ModelFactory {

    RenderableModel createCube() {
        RenderableModel cube;
        cube.name = "Cube";
        cube.points = {
            Vec3{-0.5f, -0.5f, -0.5f}, Vec3{0.5f, -0.5f, -0.5f}, Vec3{0.5f, 0.5f, -0.5f}, Vec3{-0.5f, 0.5f, -0.5f},
            Vec3{-0.5f, -0.5f, 0.5f},  Vec3{0.5f, -0.5f, 0.5f},  Vec3{0.5f, 0.5f, 0.5f},  Vec3{-0.5f, 0.5f, 0.5f}
        };
        cube.edges = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4},
            {0, 4}, {1, 5}, {2, 6}, {3, 7}
        };
        return cube;
    }

    RenderableModel createLine() {
        RenderableModel line;
        line.name = "Line";
        line.points = { Vec3{-1.0f, 0.0f, 0.0f}, Vec3{1.0f, 0.0f, 0.0f} };
        line.edges = { {0, 1} };
        return line;
    }

    RenderableModel createPyramid() {
        RenderableModel pyramid;
        pyramid.name = "Pyramid";
        pyramid.points = {
            Vec3{0.0f, 0.5f, 0.0f}, Vec3{-0.5f, -0.5f, -0.5f}, Vec3{0.5f, -0.5f, -0.5f},
            Vec3{0.5f, -0.5f, 0.5f}, Vec3{-0.5f, -0.5f, 0.5f}
        };
        pyramid.edges = {
            {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 2}, {2, 3}, {3, 4}, {4, 1}
        };
        return pyramid;
    }
}