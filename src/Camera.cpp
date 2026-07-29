#include "Camera.h"

void Camera::MoveBy(const float x, const float y) {
    cameraOffset.x += x;
    cameraOffset.y += y;
}

Position Camera::WorldToScreen(const Position worldPos) const {
    return {worldPos.x + cameraOffset.x, worldPos.y + cameraOffset.y};
}

Position Camera::ScreenToWorld(const Position screenPos) const {
    return {screenPos.x - cameraOffset.x, screenPos.y - cameraOffset.y};
}
