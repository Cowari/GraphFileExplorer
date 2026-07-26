#include "Node.h"

void Node::SetParent(const size_t index) {
    parentIndex = index;
}

bool Node::IsCursorInside(Position cursorPosition, float radius) const {
    float dx = position.x - cursorPosition.x;
    float dy = position.y - cursorPosition.y;
    float distanceSquared = dx * dx + dy * dy;

    return distanceSquared <= radius * radius;
}

Position Node::GetPosition() const {
    return position;
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