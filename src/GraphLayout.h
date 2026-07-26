#pragma once
#include <vector>
#include <string>
#include "Node.h"

class GraphLayout {
    public:
    explicit GraphLayout(const Position position) : pivotPos(position) { }

    void AddNode(Position nodePosition, bool isDir, std::string nodeName);
    void AddChildInOrbit(size_t parentIndex, float radius, float angleDeg, bool isDir, std::string nodeName);

    [[nodiscard]] const std::vector<Node>& GetNodes() const;
    [[nodiscard]] std::optional<size_t> GetNodeIndexAtPosition(Position posXY) const;
    void PrintAllNodes() const; // for debug

    private:
    std::vector<Node> allNodes;
    const Position pivotPos;
};
