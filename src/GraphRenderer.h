#pragma once
#include <imgui.h>

#include "GraphLayout.h"

class GraphRenderer {
public:
    void Render(const GraphLayout& layout) const;
private:
    // TODO:
    // void drawConnection(Node1, Node2) - нарисует линию от одной ноды к другой
    void DrawConnection(ImDrawList* bgDrawList, const Node& node1, const Node& node2) const;
};