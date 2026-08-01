#pragma once

#include "GraphRenderer.h"
#include "GraphLayout.h"

#include <imgui.h>

class WindowManager {
public:
    void DrawWindows(GraphLayout& layout, const Camera& camera, const ImVec2& displaySize);

private:
    void DrawGraph(const ImGuiWindowFlags& flags, GraphLayout& layout, const Camera& camera, const ImVec2& displaySize);

    GraphRenderer graphRenderer;
};
