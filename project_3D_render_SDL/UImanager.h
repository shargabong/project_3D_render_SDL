#pragma once

#include "imgui.h"
#include "Render.h"
#include <vector>
#include <string>
#include <functional>

class UIManager
{
public:
    UIManager();

    void initialize(SDL_Window* window, SDL_Renderer* renderer);
    void shutdown();
    void processEvent(SDL_Event* event);
    void newFrame();

    void renderUI(
        Renderer3D& renderer,
        RenderableModel& currentLoadedModel,
        bool modelIsLoaded,
        std::function<void(const std::string&)> loadObjRequestCallback
    );

    void present(SDL_Renderer* renderer);

private:
    void applyCustomStyle();
};