#include "ModelFactory.h"

namespace ModelFactory {

    RenderableModel createCube() {
        return {
            "Cube",
            { // точки
                {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f},
                {-0.5f, -0.5f, 0.5f},  {0.5f, -0.5f, 0.5f},  {0.5f, 0.5f, 0.5f},  {-0.5f, 0.5f, 0.5f}
            },
            { // ребра
                {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4},
                {0, 4}, {1, 5}, {2, 6}, {3, 7}
            }
        };
    }

    RenderableModel createLine() {
        return {
            "Line",
            { {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },
            { {0, 1} }
        };
    }

    RenderableModel createPyramid() {
        return {
            "Pyramid",
            { // точки
                {0.0f, 0.5f, 0.0f}, {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
                {0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}
            },
            { // ребра
                {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 2}, {2, 3}, {3, 4}, {4, 1}
            }
        };
    }

}