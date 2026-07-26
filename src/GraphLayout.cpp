#include "GraphLayout.h"

#include <cmath>
#include <numbers>
#include <iostream> //debug


void GraphLayout::AddNode(const Position nodePosition, const bool isDir, std::string nodeName) {
    const Position position = {pivotPos.x + nodePosition.x, pivotPos.y + nodePosition.y};
    allNodes.emplace_back(position, isDir, std::move(nodeName));
}

void GraphLayout::AddChildInOrbit(const size_t parentIndex, const float radius, float angleDeg, const bool isDir, std::string nodeName) {
    if (parentIndex >= allNodes.size()) return;

    constexpr float DEG_TO_RAD = std::numbers::pi_v<float> / 180.0f; //for optimize
    float radians = angleDeg * DEG_TO_RAD;
    const Position originPos = allNodes[parentIndex].GetPosition();
    const Position newNodePos = {
        originPos.x + radius * std::cos(radians),
        originPos.y + radius * std::sin(radians)
    };
    allNodes.emplace_back(newNodePos, isDir, std::move(nodeName));

    Node& newNode = allNodes.back();
    newNode.SetParent(parentIndex);
}

std::optional<size_t> GraphLayout::GetNodeIndexAtPosition(Position posXY) const{
    for (size_t i = allNodes.size(); i > 0; --i) {
        size_t idx = i - 1;
        if (allNodes[idx].IsCursorInside(posXY, allNodes[idx].GetRadius())) {
            return idx;
        }
    }
    return std::nullopt;
}

void GraphLayout::PrintAllNodes() const {
    for (auto& node : allNodes) {
        auto [x, y] = node.GetPosition();
        std::cout << node.GetPath() << ":\n\t x: " << x << "\ty: " << y << "\n\t Parent: " << node.GetParentIndex().value_or(0) << std::endl;
    }
}

const std::vector<Node>& GraphLayout::GetNodes() const {
    return allNodes;
}
