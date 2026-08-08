#include "Node.h"

void Node::SetParent(const size_t idx) {
    parentIndex = idx;
}

void Node::SetIndex(const size_t idx) {
    index = idx;
}

void Node::SetPath(std::string newPath) {
    path = std::move(newPath);
}

void Node::SetVelocity(const Position v) {
    velocity = v;
}

void Node::SetLocalPosition(const Position localPos) {
    localPosition = localPos;
}

void Node::SetOpened(const bool opened) {
    if (isDirectory == false) return;
    isOpen = opened;
}

void Node::UpdatePosition(const Position parentWorldPos) {
    worldPosition = Position{
        parentWorldPos.x + localPosition.x,
        parentWorldPos.y + localPosition.y
    };
}

bool Node::IsCursorInside(const Position cursorPosition, const float radius) const {
    const float dx = worldPosition.x - cursorPosition.x;
    const float dy = worldPosition.y - cursorPosition.y;
    const float distanceSquared = dx * dx + dy * dy;

    return distanceSquared <= radius * radius;
}

Position Node::GetWorldPosition() const {
    return worldPosition;
}

Position Node::GetLocalPosition() const {
    return localPosition;
}

std::string Node::GetName() const {
    const size_t separatorIndex = path.find_last_of('/');
    return (separatorIndex == std::string::npos) ? path : path.substr(separatorIndex+1) ;
}

Position Node::GetVelocity() const {
    return velocity;
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
