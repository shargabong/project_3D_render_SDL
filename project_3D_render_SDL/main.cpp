#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include "Render.h"
#include "UIManager.h"
#include "ModelFactory.h" 

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Enhanced 3D Object Viewer", 1280, 720, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* gameRenderer = SDL_CreateRenderer(window, nullptr);
    if (!gameRenderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::vector<RenderableModel> availableModels;
    availableModels.push_back(ModelFactory::createCube());
    availableModels.push_back(ModelFactory::createLine());
    availableModels.push_back(ModelFactory::createPyramid());

    Renderer3D renderer3DInstance(window, gameRenderer);
    UIManager uiManagerInstance;

    uiManagerInstance.initialize(window, gameRenderer);

    int currentModelIndex = -1;
    bool modelIsLoaded = false;
    bool running = true;
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    float deltaTime = 0.0f;

    if (!availableModels.empty()) {
        currentModelIndex = 0;
        renderer3DInstance.loadModel(availableModels[currentModelIndex]);
        modelIsLoaded = true;
    }

    while (running) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastFrameTime;
        deltaTime = elapsed.count();
        lastFrameTime = currentTime;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            uiManagerInstance.processEvent(&event);
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_WINDOW_RESIZED || event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
                int w, h;
                SDL_GetWindowSize(window, &w, &h);
                renderer3DInstance.windowWidth = w;
                renderer3DInstance.windowHeight = h;
            }
        }

        renderer3DInstance.updateRotation(deltaTime);

        uiManagerInstance.newFrame();
        uiManagerInstance.renderUI(
            renderer3DInstance,
            availableModels,
            currentModelIndex,
            modelIsLoaded
        );

        SDL_SetRenderDrawColor(gameRenderer,
            static_cast<Uint8>(renderer3DInstance.backgroundColor.x * 255),
            static_cast<Uint8>(renderer3DInstance.backgroundColor.y * 255),
            static_cast<Uint8>(renderer3DInstance.backgroundColor.z * 255),
            SDL_ALPHA_OPAQUE);
        SDL_RenderClear(gameRenderer);

        if (modelIsLoaded) {
            renderer3DInstance.renderSceneContent();
        }

        uiManagerInstance.present(gameRenderer);
        SDL_RenderPresent(gameRenderer);
    }

    uiManagerInstance.shutdown();
    SDL_DestroyRenderer(gameRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}