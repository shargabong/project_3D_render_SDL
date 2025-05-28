#pragma once
#include "Render.h"
#include <string>

namespace ModelLoader {
    bool loadObj(const std::string& filepath, RenderableModel& model);
}