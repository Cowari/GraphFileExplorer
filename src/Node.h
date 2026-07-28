#pragma once
#include <optional>
#include <string>
#include <utility>

struct Position {
    float x, y;
};

class Node {
public:
    Node(const size_t index,const Position position, const bool isDir, std::string path) :
    index(index), position(position), isDirectory(isDir), path(std::move(path)) { }

    void SetParent(size_t idx);

    [[nodiscard]] bool IsCursorInside(Position cursorPosition, float radius) const;
    [[nodiscard]] Position GetPosition() const;
    [[nodiscard]] size_t GetIndex() const;
    [[nodiscard]] float GetRadius() const;
    [[nodiscard]] bool IsDirectory() const;
    [[nodiscard]] const std::string& GetPath() const;
    [[nodiscard]] std::optional<size_t> GetParentIndex() const;

private:
    Position position;
    float sizeRadius = 16.f;
    bool isDirectory;
    std::string path;
    size_t index = 0;
    std::optional<size_t> parentIndex  = std::nullopt;
};
