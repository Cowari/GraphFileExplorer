#pragma once
#include <string>
#include <utility>

struct Position {
    float x, y;
};

class Node {
public:
    Node(const Position position, const bool isDir, std::string path) :
    position(position), isDirectory(isDir), path(std::move(path)) { }

    void SetParent(size_t index);

    [[nodiscard]] Position GetPosition() const;
    [[nodiscard]] bool IsDirectory() const;
    [[nodiscard]] const std::string& GetPath() const;
    [[nodiscard]] size_t GetParentIndex() const;

private:
    Position position;
    bool isDirectory;
    std::string path;
    size_t parentIndex  = static_cast<size_t>(-1);
};
