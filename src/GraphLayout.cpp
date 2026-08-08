#include "GraphLayout.h"

#include <random>
#include <cmath>
#include <numbers>
#include <iostream> //debug
#include <unordered_map>
#include <unordered_set>

void GraphLayout::SetCenter(const Position pos) {
    pivotPos = pos;
    UpdateNodePositions();
}

void GraphLayout::AddMainNode(std::string nodePath) {
    allNodes.emplace_back(std::nullopt, pivotPos, allNodes.size(), Position{}, true, std::move(nodePath));
}

void GraphLayout::AddChildInOrbit(const size_t parentIndex, const bool isDir, std::string nodePath) {
    constexpr float DEG_TO_RAD = std::numbers::pi_v<float> / 180.0f;
    if (parentIndex >= allNodes.size()) return;

    static std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> distribution{0.f, 360.f};
    float angleDeg = distribution(rng);

    const float radians = angleDeg * DEG_TO_RAD;

    const Position parentPos = allNodes[parentIndex].GetWorldPosition();
    const Position newNodeLocalPos = Position{
        .x = 20.f * std::cos(radians),
        .y = 20.f * std::sin(radians)
    };
    allNodes.emplace_back(parentIndex, parentPos, allNodes.size(), newNodeLocalPos, isDir, std::move(nodePath));
}

void GraphLayout::RemoveSubtree(const size_t rootIdx) {
    selectedNodeIndex = std::nullopt;
    std::unordered_set<size_t> toDelete{rootIdx};

    for (size_t i = 0; i < allNodes.size(); ++i) {
        if (const auto parentIdx = allNodes[i].GetParentIndex()) {
            if (toDelete.contains(*parentIdx)) {
                toDelete.insert(i);
            }
        }
    }

    std::vector<Node> newNodes = {};
    newNodes.reserve(allNodes.size() - toDelete.size());

    std::unordered_map<std::size_t, std::size_t> remap{}; // oldIndex -> newIndex

    for (size_t i = 0; i < allNodes.size(); ++i) {
        if (toDelete.contains(i)) continue;

        remap[i] = remap.size();

        newNodes.emplace_back(std::move(allNodes[i]));
    }

    for (size_t i = 0; i < newNodes.size(); ++i) {
        const auto oldParentIdx = newNodes[i].GetParentIndex();

        newNodes[i].SetIndex(i);
        if (oldParentIdx) {
            const size_t newParentIdx = remap.at(*oldParentIdx);
            newNodes[i].SetParent(newParentIdx);
        }
    }

    allNodes = std::move(newNodes);
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

void GraphLayout::UpdatePhysics(const float deltaTime) {
    std::vector<Position> forces(allNodes.size(), Position{});

    for (size_t i = 0; i < allNodes.size(); ++i) {
        auto [x1, y1] = allNodes[i].GetWorldPosition();

        for (size_t j = i + 1; j < allNodes.size(); ++j) {
            auto [x2, y2] = allNodes[j].GetWorldPosition();
            const float directX = x1 - x2, directY = y1 - y2;

            const float distSquared = std::max(directX*directX + directY*directY, 1.f);
            const float dist = std::sqrt(distSquared);

            const float repulsion = 20000.f / distSquared;
            // vector normalization
            const float forceX = directX / dist * repulsion;
            const float forceY = directY / dist * repulsion;
            forces[i].x += forceX; forces[i].y += forceY;
            forces[j].x -= forceX; forces[j].y -= forceY;
        }
    }

    for (size_t i = 0; i < allNodes.size(); ++i) {
        constexpr float restLength = 60.f;
        constexpr float stiffness = 40.f;

        if (auto parentIdx = allNodes[i].GetParentIndex()) {
            auto [x1, y1] = allNodes[*parentIdx].GetWorldPosition();
            auto [x2, y2] = allNodes[i].GetWorldPosition();
            const float directX = x1 - x2, directY = y1 - y2;

            const float dist = std::max(std::sqrt(directX*directX + directY*directY), 0.001f);
            const float diff = dist - restLength;
            const float forceX = (directX / dist) * diff * stiffness;
            const float forceY = (directY / dist) * diff * stiffness;

            forces[i].x += forceX;
            forces[i].y += forceY;
        }
    }

    for (size_t i = 0; i < allNodes.size(); ++i) {
        constexpr float damping = 0.85f;
        if (!allNodes[i].GetParentIndex()) continue;

        Position velocity = allNodes[i].GetVelocity();
        velocity.x = (velocity.x + forces[i].x * deltaTime) * damping;
        velocity.y = (velocity.y + forces[i].y * deltaTime) * damping;
        allNodes[i].SetVelocity(velocity);

        Position local = allNodes[i].GetLocalPosition();
        local.x += velocity.x * deltaTime;
        local.y += velocity.y * deltaTime;
        allNodes[i].SetLocalPosition(local);
    }

    UpdateNodePositions();
}

void GraphLayout::SetNodeParent(const size_t nodeIndex, const size_t newParentIndex) {
    if (nodeIndex >= allNodes.size() || newParentIndex >= allNodes.size()) return;
    allNodes[nodeIndex].SetParent(newParentIndex);
}

void GraphLayout::SetNodePath(const size_t nodeIndex, const std::string &newPath) {
    if (nodeIndex >= allNodes.size()) return;
    allNodes[nodeIndex].SetPath(newPath);
}

void GraphLayout::SetSelectedNode(const size_t idx) {
    selectedNodeIndex = idx;
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

std::optional<size_t> GraphLayout::GetSelectedNodeIndex() const {
    return selectedNodeIndex;
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
