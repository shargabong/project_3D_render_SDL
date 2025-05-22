#pragma once

#include "imgui.h"
#include "Render.h"
#include <vector>
#include <string>

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
        const std::vector<RenderableModel>& availableModels,
        int& currentModelIndex,
        bool& modelIsLoaded,
        bool& quitFlag
    );

    void present(SDL_Renderer* renderer);

private:

    void applyCustomStyle();

};