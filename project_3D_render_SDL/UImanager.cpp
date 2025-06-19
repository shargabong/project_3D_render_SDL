#include "UIManager.h"
#include "imgui/backends/imgui_impl_sdl3.h"
#include "imgui/backends/imgui_impl_sdlrenderer3.h"
#include <SDL3/SDL.h>
#include <iostream>
#include "tinyfiledialogs.h"

void UIManager::applyCustomStyle() {
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
    colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.12f, 0.13f, 0.98f);
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
    colors[ImGuiCol_CheckMark] = ImVec4(0.50f, 0.70f, 0.90f, 1.00f);
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

UIManager::UIManager() {
}

void UIManager::initialize(SDL_Window* window, SDL_Renderer* renderer) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    applyCustomStyle();
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
}

void UIManager::shutdown() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::processEvent(SDL_Event* event) {
    ImGui_ImplSDL3_ProcessEvent(event);
}

void UIManager::newFrame() {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void UIManager::renderUI(
    Renderer3D& renderer,
    RenderableModel& currentLoadedModel,
    bool modelIsLoaded,
    std::function<void(const std::string&)> loadObjRequestCallback
) {
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("Controls");

    if (ImGui::CollapsingHeader("Model Selection", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::Button("Load OBJ File...")) {
            char const* lTheOpenFileName;
            char const* lFilterPatterns[1] = { "*.obj" };

            lTheOpenFileName = tinyfd_openFileDialog(
                "Open OBJ File",
                "",
                1,
                lFilterPatterns,
                "OBJ files",
                0
            );

            if (lTheOpenFileName) {
                std::cout << "User selected OBJ file: " << lTheOpenFileName << std::endl;
                loadObjRequestCallback(std::string(lTheOpenFileName));
            }
            else {
                std::cout << "User cancelled OBJ file selection." << std::endl;
            }
        }

        ImGui::Separator();

        if (modelIsLoaded) {
            ImGui::Text("Current model: %s", currentLoadedModel.name.c_str());
        }
        else {
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.8f, 1.0f), "No model loaded");
            ImGui::Text("Use 'Load OBJ File...' to select a model.");
        }
    }
    ImGui::Separator();

    if (ImGui::CollapsingHeader("View Controls", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::ColorEdit3("Model Color", &renderer.modelColor.x);
        ImGui::ColorEdit3("Background Color", &renderer.backgroundColor.x);
        ImGui::Separator();
        ImGui::Text("Camera Settings:");
        ImGui::SliderFloat("Field of View", &renderer.getCamera().fov, 10.0f, 120.0f, "%.0f deg");
        if (ImGui::Button("Reset View")) {
            renderer.resetView();
        }
    }
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Rotation Controls")) {
        ImGui::Checkbox("Auto-rotate", &renderer.autoRotateModel);
        if (renderer.autoRotateModel) {
            ImGui::Indent();
            ImGui::Checkbox("Rotate X", &renderer.rotateModelXEnabled); ImGui::SameLine(120);
            ImGui::SliderFloat("Speed X", &renderer.modelRotationSpeedX, -2.0f, 2.0f);

            ImGui::Checkbox("Rotate Y", &renderer.rotateModelYEnabled); ImGui::SameLine(120);
            ImGui::SliderFloat("Speed Y", &renderer.modelRotationSpeedY, -2.0f, 2.0f);

            ImGui::Checkbox("Rotate Z", &renderer.rotateModelZEnabled); ImGui::SameLine(120);
            ImGui::SliderFloat("Speed Z", &renderer.modelRotationSpeedZ, -2.0f, 2.0f);
            ImGui::Unindent();
        }
        if (ImGui::Button("Reset Rotation Angles")) {
            renderer.resetRotationAngles();
        }
    }
    ImGui::Separator();

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}

void UIManager::present(SDL_Renderer* renderer) {
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
}