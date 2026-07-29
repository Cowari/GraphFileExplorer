#pragma once
#include <vector>
#include <string>
#include "Node.h"

class GraphLayout {
    public:
    explicit GraphLayout(const Position position) : pivotPos(position) { }

    void AddNode(Position nodePosition, bool isDir, std::string nodePath);
    void AddChildInOrbit(size_t parentIndex, bool isDir, std::string nodePath);
    void SetOpened(size_t idx, bool open);

    [[nodiscard]] const std::vector<Node>& GetNodes() const;
    [[nodiscard]] std::optional<size_t> GetNodeIndexAtPosition(Position posXY) const;
    [[nodiscard]] bool IsNodeExists(const std::string& nodePath) const;
    void PrintAllNodes() const; // for debug

    private:
    std::vector<Node> allNodes;
    const Position pivotPos;
};
