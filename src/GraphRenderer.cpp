#include "GraphRenderer.h"

void GraphRenderer::DrawConnection(ImDrawList* bgDrawList, const Position nodePos1, const Position nodePos2) const {
    auto [x1, y1] = nodePos1;
    auto [x2, y2] = nodePos2;
    bgDrawList->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(130,200,210,150), 5.f);
}

void GraphRenderer::Render(const GraphLayout& layout, const Camera& camera) const {
    ImDrawList* bgDrawList = ImGui::GetBackgroundDrawList();
    const auto& allNodes = layout.GetNodes();

    for (const auto& node : allNodes) {
        if (std::optional<size_t> parentIndex = node.GetParentIndex()) {
            const Position nodePos = node.GetPosition();
            auto [drawPosX, drawPosY] = camera.WorldToScreen(nodePos);
            DrawConnection(bgDrawList, Position{drawPosX, drawPosY}, camera.WorldToScreen(allNodes[*parentIndex].GetPosition()));

            // debug
            std::string parentIdxString = "P" + std::to_string(*parentIndex);
            bgDrawList->AddText(
                ImVec2(drawPosX-8.f, drawPosY-30.f),
                IM_COL32(255,255,255,255),
                parentIdxString.data(),
                parentIdxString.data() + parentIdxString.size()
                );
        }
    }

    for ( const auto& node : allNodes ) {
        const Position nodePos = node.GetPosition();
        const std::string& nodeText = node.GetPath();
        const ImU32 color = node.IsDirectory() ? IM_COL32(50,150,200,255) : IM_COL32(90,175,150,255);

        auto [drawPosX, drawPosY] = camera.WorldToScreen(nodePos);
        bgDrawList->AddCircleFilled(ImVec2(drawPosX, drawPosY), node.GetRadius(), color);
        bgDrawList->AddText(
            ImVec2(drawPosX-12.f, drawPosY+15.f),
            IM_COL32(255,255,255,255),
            nodeText.data(),
            nodeText.data() + nodeText.size()
            );

        // debug
        std::string indexStr = std::to_string(node.GetIndex());
        bgDrawList->AddText(
            ImVec2(drawPosX-4.f, drawPosY-7.f),
            IM_COL32(50,50,50,255),
            indexStr.data(),
            indexStr.data() + indexStr.size()
            );
    }
}
