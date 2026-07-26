#include "GraphRenderer.h"

void GraphRenderer::DrawConnection(ImDrawList* bgDrawList, const Node& node1, const Node& node2) const {
    auto [x1, y1] = node1.GetPosition();
    auto [x2, y2] = node2.GetPosition();
    bgDrawList->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(130,200,210,150), 5.f);
}

void GraphRenderer::Render(const GraphLayout& layout) {
    ImDrawList* bgDrawList = ImGui::GetBackgroundDrawList();
    const auto& allNodes = layout.GetNodes();

    for (const auto& node : allNodes) {
        size_t parentIndex = node.GetParentIndex();
        if (parentIndex < allNodes.size()) {
            auto [x, y] = node.GetPosition();
            DrawConnection(bgDrawList, node, allNodes[parentIndex]);
            // debug
            std::string parentIdxString = "P" + std::to_string(parentIndex);
            bgDrawList->AddText(
                ImVec2(x-8.f, y-30.f),
                IM_COL32(255,255,255,255),
                parentIdxString.data(),
                parentIdxString.data() + parentIdxString.size()
                );
        }
    }

    for ( size_t i = 0; const auto& node : allNodes ) {
        auto [x, y] = node.GetPosition();
        const std::string& nodeText = node.GetPath();
        const ImU32 color = node.IsDirectory() ? IM_COL32(50,150,200,255) : IM_COL32(90,175,150,255);

        bgDrawList->AddCircleFilled(ImVec2(x, y), 16.0f, color);
        bgDrawList->AddText(
            ImVec2(x-12.f, y+15.f),
            IM_COL32(255,255,255,255),
            nodeText.data(),
            nodeText.data() + nodeText.size()
            );

        // debug
        std::string indexStr = std::to_string(i);
        bgDrawList->AddText(
            ImVec2(x-4.f, y-7.f),
            IM_COL32(50,50,50,255),
            indexStr.data(),
            indexStr.data() + indexStr.size()
            );
        i++;
    }
}
