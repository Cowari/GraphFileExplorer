#include "WindowManager.h"

void WindowManager::DrawWindows(GraphLayout &layout, const Camera &camera, NodeNamePopup& nodeNamePopup, const ImVec2 &displaySize) {
    constexpr ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

    DrawGraph(windowFlags, layout, camera, displaySize);

    if (nodeNamePopup.IsOpen()) {
        nodeNamePopup.Draw(windowFlags, layout);
    }
}

void WindowManager::DrawGraph(const ImGuiWindowFlags& flags, GraphLayout& layout, const Camera& camera, const ImVec2& displaySize) const {
    ImGui::SetNextWindowPos(ImVec2{0.f, 0.f});
    ImGui::SetNextWindowSize(displaySize);

    // 'flags | ImGuiWindowFlags_NoInputs' - Prevent the graph window from covering other windows on click.
    ImGui::Begin("Graph", nullptr, flags | ImGuiWindowFlags_NoInputs);
    graphRenderer.Render(layout, camera);
    const ImVec2 windowCenter = {
        ImGui::GetWindowPos().x + ImGui::GetWindowSize().x * 0.5f,
        ImGui::GetWindowPos().y + ImGui::GetWindowSize().y * 0.5f,
    };

    layout.SetCenter(Position{windowCenter.x, windowCenter.y});
    ImGui::End();
}
