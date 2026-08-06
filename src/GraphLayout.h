#pragma once
#include <vector>
#include <string>
#include "Node.h"

class GraphLayout {
    public:
    GraphLayout() = default;

    void SetCenter(Position pos);

    void AddMainNode(std::string nodePath);
    void AddChildInOrbit(size_t parentIndex, bool isDir, std::string nodePath);
    void UpdateDescendantPaths(const std::string& oldPrefix, const std::string& newPrefix);
    void UpdateNodePositions();
    void UpdatePhysics(float deltaTime);
    void SetNodeParent(size_t nodeIndex, size_t newParentIndex);
    void SetNodePath(size_t nodeIndex, const std::string &newPath);
    void SetSelectedNode(size_t idx);
    void SetOpened(size_t idx, bool open);

    [[nodiscard]] const std::vector<Node>& GetNodes() const;
    [[nodiscard]] std::optional<size_t> GetNodeIndexAtPosition(Position posXY) const;
    [[nodiscard]] std::optional<size_t> GetNodeIndexByPath(const std::string& path) const;
    [[nodiscard]] std::optional<size_t> GetSelectedNodeIndex() const;
    [[nodiscard]] bool IsNodeExists(const std::string& nodePath) const;
    void PrintAllNodes() const; // for debug
    void PrintNodeInfo(const Node& node) const;

    private:
    std::vector<Node> allNodes;
    std::optional<size_t> selectedNodeIndex;
    Position pivotPos {};
};
