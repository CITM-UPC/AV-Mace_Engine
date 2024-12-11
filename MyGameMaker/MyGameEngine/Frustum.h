#pragma once

#include "BoundingBox.h"
#include "types.h"
#include <array>
#include "Camera.h"

class Frustum {
public:
    Frustum() = default;

    std::array<glm::vec3, 8> calculateFrustumCorners(const Camera& camera);
};

