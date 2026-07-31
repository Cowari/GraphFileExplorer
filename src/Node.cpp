#include "Node.h"

void Node::SetParent(const size_t idx) {
    parentIndex = idx;
}

void Node::SetPath(std::string newPath) {
    path = std::move(newPath);
}

void Node::SetOpened(const bool opened) {
    if (isDirectory == false) return;
    isOpen = opened;
}

bool Node::IsCursorInside(const Position cursorPosition, const float radius) const {
    const float dx = position.x - cursorPosition.x;
    const float dy = position.y - cursorPosition.y;
    const float distanceSquared = dx * dx + dy * dy;

    return distanceSquared <= radius * radius;
}

Position Node::GetPosition() const {
    return position;
}

size_t Node::GetIndex() const {
    return index;
}

float Node::GetRadius() const {
    return sizeRadius;
}

bool Node::IsDirectory() const {
    return isDirectory;
}

bool Node::IsOpen() const {
    if (isDirectory == false) return false;

    return isOpen;
}

const std::string& Node::GetPath() const {
    return path;
}

std::optional<size_t> Node::GetParentIndex() const {
    return parentIndex;
}
