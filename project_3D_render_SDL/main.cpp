#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <iostream>
#include "Render.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

// Функция создания куба
RenderableModel createCube() {
    return {
        "Cube",
        { // точки
            {-0.5f, -0.5f, -0.5f},
            {0.5f, -0.5f, -0.5f},
            {0.5f, 0.5f, -0.5f},
            {-0.5f, 0.5f, -0.5f},
            {-0.5f, -0.5f, 0.5f},
            {0.5f, -0.5f, 0.5f},
            {0.5f, 0.5f, 0.5f},
            {-0.5f, 0.5f, 0.5f}
        },
        { // ребра
            {0, 1},
            {1, 2},
            {2, 3},
            {3, 0},
            {4, 5},
            {5, 6},
            {6, 7},
            {7, 4},
            {0, 4},
            {1, 5},
            {2, 6},
            {3, 7}
        }
    };
}

// Функция создания линии
RenderableModel createLine() {
    return {
        "Line",
        { // точки
            {-1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f}
        },
        { // ребра
            {0, 1}
        }
    };
}

// Функция создания пирамиды
RenderableModel createPyramid() {
    return {
        "Pyramid",
        { // точки
            {0.0f, 0.5f, 0.0f},
            {-0.5f, -0.5f, -0.5f},
            {0.5f, -0.5f, -0.5f},
            {0.5f, -0.5f, 0.5f},
            {-0.5f, -0.5f, 0.5f}
        },
        { // ребра
            {0, 1},
            {0, 2},
            {0, 3},
            {0, 4},
            {1, 2},
            {2, 3},
            {3, 4},
            {4, 1}
        }
    };
}

int main(int argc, char* argv[]) {
    // Инициализация SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Создание окна
    SDL_Window* window = SDL_CreateWindow("3D Object Selector", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Создание рендерера
    SDL_Renderer* gameRenderer = SDL_CreateRenderer(window, nullptr);
    if (!gameRenderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Создание вектора моделей
    std::vector<RenderableModel> availableModels;
    availableModels.push_back(createCube());
    availableModels.push_back(createLine());
    availableModels.push_back(createPyramid());

    // Создание рендерера 3D
    Renderer3D renderer3DInstance(window, gameRenderer);

    // Инициализация переменных
    int currentModelIndex = 0;
    bool modelIsLoaded = false;

    // Инициализация ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();

    // Инициализация ImGui
    ImGui_ImplSDL3_InitForSDLRenderer(window, gameRenderer);
    ImGui_ImplSDLRenderer3_Init(gameRenderer);

    // Основной цикл программы
    bool running = true;

    while (running) {
        // Обработка событий
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // Рендеринг и обновление
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Отрисовка интерфейса
        ImGui::Begin("Controls");
        if (ImGui::CollapsingHeader("Model Selection", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (availableModels.empty()) {
                ImGui::Text("No models available");
            }
            else {
                ImGui::Text("Available models:");
                for (size_t i = 0; i < availableModels.size(); ++i) {
                    if (ImGui::RadioButton(availableModels[i].name.c_str(), currentModelIndex == static_cast<int>(i))) {
                        currentModelIndex = i;
                        renderer3DInstance.loadModel(availableModels[currentModelIndex]);
                        modelIsLoaded = true;
                    }
                }
            }
        }

        // Отрисовка текущей модели
        if (modelIsLoaded && currentModelIndex != -1) {
            ImGui::Separator();
            ImGui::Text("Current model: %s", availableModels[currentModelIndex].name.c_str());
        }
        else {
            ImGui::Text("No model selected");
        }

        // Кнопка выхода
        ImGui::Separator();
        if (ImGui::Button("Quit")) {
            running = false;
        }

        // Информация о производительности
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        // Рендеринг 3D-сцены
        renderer3DInstance.render();

        // Рендеринг ImGui
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(),gameRenderer);
        SDL_RenderPresent(gameRenderer);
    }

    // Выход из программы
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(gameRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

//(олд версия)
//#include <SDL3/SDL.h>
//#include <vector>
//#include "Render.h"
//
//#define SDL_RENDERER_ACCELERATED 0x00000002
//
////line 
//std::vector<Point3D> points{ 
//	Point3D{ -1.0f, -1.0f, -1.0f }, 
//	Point3D{ -1.0f, -1.0f, 1.0f }, 
//};
//
//std::vector<Vertex> vertices{ 
//	Vertex{ 0, 1 },
//};
//
////cube
////std::vector<Point3D> points{
////	Point3D{ -1.0f, -1.0f, -1.0f },
////	Point3D{ -1.0f, -1.0f, 1.0f },
////	Point3D{ 1.0f, -1.0f, -1.0f },
////	Point3D{ -1.0f, 1.0f, -1.0f },
////	Point3D{ -1.0f, 1.0f, 1.0f },
////	Point3D{ 1.0f, -1.0f, 1.0f },
////	Point3D{ 1.0f, 1.0f, -1.0f },
////	Point3D{ 1.0f, 1.0f, 1.0f }
////};
////
////std::vector<Vertex> vertices{
////	Vertex{ 0, 1 },
////	Vertex{ 0, 2 },
////	Vertex{ 0, 3 },
////	Vertex{ 2, 5 },
////	Vertex{ 3, 6 },
////	Vertex{ 3, 4 },
////	Vertex{ 4, 7 },
////	Vertex{ 6, 7 },
////	Vertex{ 7, 5 },
////	Vertex{ 5, 1 },
////	Vertex{ 4, 1 },
////	Vertex{ 2, 6 },
////};
//
////diamond
////std::vector<Point3D> points{
////    Point3D{ 0.0f, -1.0f, 0.0f },
////    Point3D{ -1.0f, 0.5f, 0.0f },
////	Point3D{ 1.0f, 0.5f, 0.0f },
////    Point3D{ -0.7f, 0.8f, 0.0f },
////    Point3D{ 0.7f, 0.8f, 0.0f },
////};
////
////std::vector<Vertex> vertices{
////    Vertex{ 0, 1 },
////    Vertex{ 0, 2 },
////    Vertex{ 1, 3 },
////    Vertex{ 2, 4 },
////    Vertex{ 3, 4 },
////};
//
//int main(int argc, char* argv[]) {
//
//	SDL_Window* window;
//	SDL_Renderer* renderer;
//	window = SDL_CreateWindow("3D Renderer", 800, 600, 0);
//	renderer = SDL_CreateRenderer(window, 0);
//
//	bool running = true;
//
//	Renderer3D renderer3D1(window, renderer, points, vertices);
//
//	while (running) {
//		SDL_Event event;
//		if (SDL_PollEvent(&event)) {
//			if (event.type == SDL_EVENT_QUIT) {
//				running = false;
//				break;
//			}
//		}
//		renderer3D1.render();
//	}
//
//	return 0;
//}


//void displayModelChoices(const std::vector<RenderableModel>& models, bool isInConsoleInputMode, int currentModelIdx, bool isModelCurrentlyLoaded)
//{
//    std::cout << "\n--- Model Selection ---" << std::endl;
//
//    if (models.empty()) {
//        std::cout << "No models available to load." << std::endl;
//        return;
//    }
//
//    if (isModelCurrentlyLoaded) {
//        if (currentModelIdx >= 0 && static_cast<size_t>(currentModelIdx) < models.size()) {
//            std::cout << "Currently rendering: " << models[currentModelIdx].name << std::endl;
//        }
//        else {
//            std::cout << "Currently rendering: Unknown (error in index)" << std::endl;
//        }
//    }
//    else {
//        std::cout << "No model loaded. Please make a selection." << std::endl;
//    }
//
//    std::cout << "Available models:" << std::endl;
//    for (size_t i = 0; i < models.size(); ++i)
//    {
//        std::cout << i + 1 << ". " << models[i].name << std::endl;
//    }
//
//    if (isInConsoleInputMode) {
//        std::cout << "Enter model number to load (or 0 to cancel and use Spacebar cycling): ";
//    }
//    else {
//        if (isModelCurrentlyLoaded) {
//            std::cout << "Press Space to cycle models, or Enter to select by number: ";
//        }
//        else {
//            std::cout << "Press Space to select the first model, or Enter to select by number: ";
//        }
//    }
//}

//bool consoleInputMode = false;
//displayModelChoices(availableModels, consoleInputMode, currentModelIndex, modelIsLoaded);
//
//    while (running)
//    {
//        SDL_Event event;
//        while (SDL_PollEvent(&event))
//        {
//            if (event.type == SDL_EVENT_QUIT)
//            {
//                running = false;
//            }
//            if (event.type == SDL_EVENT_KEY_DOWN)
//            {
//                if (consoleInputMode)
//                {
//                    if (event.key.key >= SDLK_0 && event.key.key <= SDLK_9)
//                    {
//                        int choice = event.key.key - SDLK_0;
//                        if (choice == 0)
//                        {
//                            consoleInputMode = false;
//                            std::cout << "\nInput cancelled. Switched to Spacebar cycling mode." << std::endl;
//                            displayModelChoices(availableModels, consoleInputMode, currentModelIndex, modelIsLoaded);
//                        }
//                        else if (!availableModels.empty() && choice > 0 && choice <= static_cast<int>(availableModels.size()))
//                        {
//                            currentModelIndex = choice - 1;
//                            renderer3DInstance.loadModel(availableModels[currentModelIndex]);
//                            modelIsLoaded = true;
//                            consoleInputMode = false;
//                            std::cout << "\nModel '" << availableModels[currentModelIndex].name << "' selected." << std::endl;
//                            displayModelChoices(availableModels, consoleInputMode, currentModelIndex, modelIsLoaded);
//                        }
//                        else if (availableModels.empty()) {
//                            std::cout << "\nNo models to select from." << std::endl;
//                            consoleInputMode = false;
//                            displayModelChoices(availableModels, consoleInputMode, currentModelIndex, modelIsLoaded);
//                        }
//                        else {
//                            std::cout << "\nInvalid choice. Try again (1-" << availableModels.size() << ") or press 0 to cancel." << std::endl;
//                        }
//                    }
//                    else if (event.key.key == SDLK_ESCAPE)
//                    {
//                        consoleInputMode = false;
//                        std::cout << "\nInput cancelled via Escape. Switched to Spacebar cycling mode." << std::endl;
//                        displayModelChoices(availableModels, consoleInputMode, currentModelIndex, modelIsLoaded);
//                    }
//                }
//                else
//                {
//                    if (event.key.key == SDLK_SPACE)
//                    {
//                        if (!availableModels.empty())
//                        {
//                            if (!modelIsLoaded) {
//
//                            }
//                            else {
//                                currentModelIndex = (currentModelIndex + 1) % availableModels.size();
//                            }
//                            renderer3DInstance.loadModel(availableModels[currentModelIndex]);
//                            modelIsLoaded = true;
//                            std::cout << "\nSelected model: " << availableModels[currentModelIndex].name << std::endl;
//                            displayModelChoices(availableModels, consoleInputMode, currentModelIndex, modelIsLoaded);
//                        }
//                        else {
//                            std::cout << "\nNo models to cycle." << std::endl;
//                            displayModelChoices(availableModels, consoleInputMode, currentModelIndex, modelIsLoaded);
//                        }
//                    }
//                    else if (event.key.key == SDLK_RETURN || event.key.key == SDLK_KP_ENTER)
//                    {
//                        if (!availableModels.empty()) {
//                            consoleInputMode = true;
//                            std::cout << "\nEntering number selection mode." << std::endl;
//                            displayModelChoices(availableModels, consoleInputMode, currentModelIndex, modelIsLoaded);
//                        }
//                        else {
//                            std::cout << "\nNo models available to select by number." << std::endl;
//                            displayModelChoices(availableModels, consoleInputMode, currentModelIndex, modelIsLoaded);
//                        }
//                    }
//                }
//            }
//        }
//
//        renderer3DInstance.render();
//        SDL_Delay(16);
//    }
//
//    SDL_DestroyRenderer(gameRenderer);
//    SDL_DestroyWindow(window);
//    SDL_Quit();
//
//    return 0;
//}