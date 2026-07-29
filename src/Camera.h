#pragma once
#include "Node.h"

class Camera {
public:
    void MoveBy(float x, float y);

    [[nodiscard]] Position WorldToScreen(Position worldPos) const;
    [[nodiscard]] Position ScreenToWorld(Position screenPos) const;

private:
    Position cameraOffset = {.x = 0.0f, .y = 0.0f};
};
