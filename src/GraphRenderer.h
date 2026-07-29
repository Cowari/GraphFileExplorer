#pragma once
#include <imgui.h>

#include "GraphLayout.h"
#include "Camera.h"

class GraphRenderer {
public:
    void Render(const GraphLayout& layout, const Camera& camera) const;
private:
    void DrawConnection(ImDrawList* bgDrawList, Position nodePos1, Position nodePos2) const;
};