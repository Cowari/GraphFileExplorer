#include "Node.h"

void Node::SetParent(const size_t idx) {
    parentIndex = idx;
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

size_t Node::GetIndex() const { // test
    return index;
}

float Node::GetRadius() const {
    return sizeRadius;
}

bool Node::IsDirectory() const {
    return isDirectory;
}

const std::string& Node::GetPath() const {
    return path;
}

std::optional<size_t> Node::GetParentIndex() const {
    return parentIndex;
}