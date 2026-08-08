#pragma once
#include <optional>
#include <string>
#include <utility>

struct Position {
    float x, y;
};

class Node {
public:
    Node(const std::optional<size_t> parentIndex, const Position parentWorldPos, const size_t index, const Position localPos, const bool isDir, std::string path) :
    worldPosition(Position{parentWorldPos.x + localPos.x, parentWorldPos.y + localPos.y}), localPosition(localPos),
    isDirectory(isDir), path(std::move(path)), index(index), parentIndex(parentIndex) { }

    void SetParent(size_t idx);
    void SetIndex(size_t idx);
    void SetPath(std::string newPath);
    void SetVelocity(Position v);
    void SetLocalPosition(Position localPos);
    void SetOpened(bool opened);
    void UpdatePosition(Position parentWorldPos);

    [[nodiscard]] bool IsCursorInside(Position cursorPosition, float radius) const;
    [[nodiscard]] Position GetWorldPosition() const;
    [[nodiscard]] Position GetLocalPosition() const;
    [[nodiscard]] std::string GetName() const;
    [[nodiscard]] size_t GetIndex() const;
    [[nodiscard]] float GetRadius() const;
    [[nodiscard]] bool IsDirectory() const;
    [[nodiscard]] bool IsOpen() const;
    [[nodiscard]] const std::string& GetPath() const;
    [[nodiscard]] std::optional<size_t> GetParentIndex() const;
    [[nodiscard]] Position GetVelocity() const;

private:
    Position velocity{};
    Position worldPosition;
    Position localPosition;
    float sizeRadius = 16.f;
    bool isDirectory;
    bool isOpen = false;
    std::string path;
    size_t index = 0;
    std::optional<size_t> parentIndex;
};
