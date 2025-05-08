#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <iostream>
#include <chrono> // для DeltaTime
#include "Render.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

// Функции createCube, createLine, createPyramid остаются без изменений

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
        { {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} }, { {0, 1} }
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


void ApplyCustomStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(8, 4);
    style.ItemInnerSpacing = ImVec2(4, 4);
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize = 12.0f;

    style.WindowRounding = 6.0f;
    style.ChildRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.12f, 0.13f, 0.98f); // Slightly transparent for effect
    colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.16f, 0.18f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.28f, 0.30f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.31f, 0.33f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.09f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.07f, 0.07f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.15f, 0.16f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.50f, 0.70f, 0.90f, 1.00f); // A nice blue
    colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.60f, 0.80f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.50f, 0.70f, 0.90f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.30f, 0.50f, 0.70f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.60f, 0.80f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.45f, 0.65f, 0.85f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.30f, 0.50f, 0.70f, 0.31f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.40f, 0.60f, 0.80f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.45f, 0.65f, 0.85f, 1.00f);
    colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.40f, 0.58f, 0.80f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.35f, 0.55f, 0.75f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.30f, 0.50f, 0.70f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.20f, 0.25f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.28f, 0.35f, 1.00f);
//    colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
//    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Enhanced 3D Object Viewer", 1280, 720, SDL_WINDOW_RESIZABLE);
    if (!window) {
        // ... ошибка ...
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* gameRenderer = SDL_CreateRenderer(window, nullptr);
    if (!gameRenderer) {
        // ... ошибка ...
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::vector<RenderableModel> availableModels;
    availableModels.push_back(createCube());
    availableModels.push_back(createLine());
    availableModels.push_back(createPyramid());

    Renderer3D renderer3DInstance(window, gameRenderer);

    int currentModelIndex = -1; // -1 означает, что модель не выбрана изначально
    bool modelIsLoaded = false;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
//    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Включаем докинг, если хотим окна ImGui внутри главного
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Для окон вне главного окна SDL (может требовать доп. настроек)

    // ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();
    ApplyCustomStyle(); // Применяем наш кастомный стиль

    ImGui_ImplSDL3_InitForSDLRenderer(window, gameRenderer);
    ImGui_ImplSDLRenderer3_Init(gameRenderer);

    bool running = true;
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    float deltaTime = 0.0f;


    // Загружаем первую модель по умолчанию, если есть
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
            ImGui_ImplSDL3_ProcessEvent(&event);
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

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // --- UI Controls Window ---
        ImGui::Begin("Controls");

        if (ImGui::CollapsingHeader("Model Selection", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (availableModels.empty()) {
                ImGui::Text("No models available");
            }
            else {
                for (size_t i = 0; i < availableModels.size(); ++i) {
                    if (ImGui::RadioButton(availableModels[i].name.c_str(), currentModelIndex == static_cast<int>(i))) {
                        currentModelIndex = i;
                        renderer3DInstance.loadModel(availableModels[currentModelIndex]);
                        // renderer3DInstance.resetView(); // Можно сбрасывать вид при смене модели
                        modelIsLoaded = true;
                    }
                }
            }
            if (modelIsLoaded && currentModelIndex != -1) {
                ImGui::Text("Current model: %s", availableModels[currentModelIndex].name.c_str());
            }
            else {
                ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.8f, 1.0f), "No model selected");
            }
        }
        ImGui::Separator();

        if (ImGui::CollapsingHeader("View Controls", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::ColorEdit3("Model Color", &renderer3DInstance.modelColor.x);
            ImGui::ColorEdit3("Background Color", &renderer3DInstance.backgroundColor.x);
            ImGui::Separator();
            ImGui::SliderFloat("Field of View", &renderer3DInstance.fov, 10.0f, 120.0f, "%.0f deg");
            ImGui::SliderFloat("Camera Distance", &renderer3DInstance.cameraDistance, 0.5f, 20.0f, "%.1f units");
            if (ImGui::Button("Reset View")) {
                renderer3DInstance.resetView();
            }
        }
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Rotation Controls")) {
            ImGui::Checkbox("Auto-rotate", &renderer3DInstance.autoRotate);
            if (renderer3DInstance.autoRotate) {
                ImGui::Indent();
                ImGui::Checkbox("Rotate X", &renderer3DInstance.rotateXEnabled); ImGui::SameLine(120);
                ImGui::SliderFloat("Speed X", &renderer3DInstance.rotationSpeedX, -2.0f, 2.0f);

                ImGui::Checkbox("Rotate Y", &renderer3DInstance.rotateYEnabled); ImGui::SameLine(120);
                ImGui::SliderFloat("Speed Y", &renderer3DInstance.rotationSpeedY, -2.0f, 2.0f);

                ImGui::Checkbox("Rotate Z", &renderer3DInstance.rotateZEnabled); ImGui::SameLine(120);
                ImGui::SliderFloat("Speed Z", &renderer3DInstance.rotationSpeedZ, -2.0f, 2.0f);
                ImGui::Unindent();
            }
            if (ImGui::Button("Reset Rotation Angles")) { // Кнопка для сброса только углов
                renderer3DInstance.resetRotationAngles();
            }
        }
        ImGui::Separator();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        if (ImGui::Button("Quit Application")) {
            running = false;
        }
        ImGui::End(); // End Controls window

        // --- Рендеринг ---
        // Устанавливаем цвет фона из рендерера
        SDL_SetRenderDrawColor(gameRenderer,
            static_cast<Uint8>(renderer3DInstance.backgroundColor.x * 255),
            static_cast<Uint8>(renderer3DInstance.backgroundColor.y * 255),
            static_cast<Uint8>(renderer3DInstance.backgroundColor.z * 255),
            SDL_ALPHA_OPAQUE);
        SDL_RenderClear(gameRenderer);

        // Рендеринг 3D-сцены (только объекты)
        if (modelIsLoaded) {
            renderer3DInstance.renderSceneContent();
        }

        // Рендеринг ImGui поверх всего
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), gameRenderer);

        // Если используете Viewports (окна ImGui вне главного окна)
        // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        //     SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow(); // SDL_GL_GetCurrentContext для OpenGL
        //     SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        //     ImGui::UpdatePlatformWindows();
        //     ImGui::RenderPlatformWindowsDefault();
        //     SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        // }

        SDL_RenderPresent(gameRenderer);
    }

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(gameRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}