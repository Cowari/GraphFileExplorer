#include "Node.h"

void Node::SetParent(const size_t index) {
    parentIndex = index;
}

Position Node::GetPosition() const {
    return position;
}

bool Node::IsDirectory() const {
    return isDirectory;
}

const std::string& Node::GetPath() const {
    return path;
}

size_t Node::GetParentIndex() const {
    return parentIndex;
}