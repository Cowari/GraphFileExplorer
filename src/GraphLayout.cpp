#include "GraphLayout.h"

#include <cmath>
#include <numbers>
#include <iostream> //debug

void GraphLayout::SetCenter(const Position pos) {
    pivotPos = pos;
    UpdateNodePositions();
}

void GraphLayout::AddMainNode(std::string nodePath) {
    allNodes.emplace_back(std::nullopt, pivotPos, allNodes.size(), Position{}, true, std::move(nodePath));
}

void GraphLayout::AddChildInOrbit(const size_t parentIndex, const bool isDir, std::string nodePath) {
    if (parentIndex >= allNodes.size()) return;

    constexpr float DEG_TO_RAD = std::numbers::pi_v<float> / 180.0f; //for optimize
    const float radians = 16.f * DEG_TO_RAD; //hardcode angleDeg

    const Position parentPos = allNodes[parentIndex].GetWorldPosition();
    const Position newNodeLocalPos = {
        60.f * std::cos(radians), // hardcode radius
        60.f * std::sin(radians) // hardcode radius
    };
    allNodes.emplace_back(parentIndex, parentPos, allNodes.size(), newNodeLocalPos, isDir, std::move(nodePath));
}

void GraphLayout::UpdateDescendantPaths(const std::string &oldPrefix, const std::string &newPrefix) {
    for (size_t i = 0; i < allNodes.size(); ++i) {
        const std::string& currentPath = allNodes[i].GetPath();
        const bool startsWithPrefix = currentPath.starts_with(oldPrefix);
        const bool isChild = startsWithPrefix && currentPath[oldPrefix.size()] == '/';
        if (isChild) {
            const std::string& tail = currentPath.substr(oldPrefix.size());
            const std::string& newPath = newPrefix + tail;
            SetNodePath(i, newPath);
        }
    }
}

void GraphLayout::UpdateNodePositions() {
    for (size_t i = 0; i < allNodes.size(); ++i) {
        Position parentWorldPos{};
        std::optional<size_t> parentIndex = allNodes[i].GetParentIndex();
        if (parentIndex) {
            parentWorldPos = allNodes[*parentIndex].GetWorldPosition();
        } else {
            parentWorldPos = pivotPos;
        }

        allNodes[i].UpdatePosition(parentWorldPos);
    }
}

void GraphLayout::SetNodeParent(const size_t nodeIndex, const size_t newParentIndex) {
    if (nodeIndex >= allNodes.size() || newParentIndex >= allNodes.size()) return;
    allNodes[nodeIndex].SetParent(newParentIndex);
}

void GraphLayout::SetNodePath(const size_t nodeIndex, const std::string &newPath) {
    if (nodeIndex >= allNodes.size()) return;
    allNodes[nodeIndex].SetPath(newPath);
}

void GraphLayout::SetOpened(const size_t idx, const bool open) {
    if (idx >= allNodes.size() || !allNodes[idx].IsDirectory()) return;

    allNodes[idx].SetOpened(open);
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

std::optional<size_t> GraphLayout::GetNodeIndexByPath(const std::string &path) const {
    for (size_t i = 0; i < allNodes.size(); ++i) {
        if (allNodes[i].GetPath() == path) {
            return i;
        }
    }
    return std::nullopt;
}

bool GraphLayout::IsNodeExists(const std::string& nodePath) const {
    return GetNodeIndexByPath(nodePath).has_value();
}

void GraphLayout::PrintAllNodes() const {
    for (auto& node : allNodes) {
        auto [x, y] = node.GetWorldPosition();
        std::cout << node.GetPath() << '[' << node.GetIndex() << ']' <<
            ":\n\t worldX: " << x << "\tworldY: " << y <<
                "\n\t Parent: " << node.GetParentIndex().value_or(0) << std::endl;
    }
}

void GraphLayout::PrintNodeInfo(const Node &node) const {
    auto [wx, wy] = node.GetWorldPosition();
    auto [lx, ly] = node.GetLocalPosition();

    std::cout << node.GetPath() << '[' << node.GetIndex() << ']' << ":\n\t worldX: " << wx << "\tworldY: " << wy <<
        "\n\t localX: " << lx << "\tlocalY: " << ly << "\n\t Parent: " << node.GetParentIndex().value_or(0) << std::endl;
}

const std::vector<Node>& GraphLayout::GetNodes() const {
    return allNodes;
}
