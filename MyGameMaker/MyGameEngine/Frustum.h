#pragma once

#include "BoundingBox.h"
#include "types.h"
#include <array>

class Frustum {
public:
    Frustum() = default;

    // Array of six planes representing the frustum
    std::array<Plane, 6> planes;

    // Method to set the frustum planes from a view-projection matrix
    void setFrustumPlanes(const glm::dmat4& viewProjMatrix);

    // Method to test if a bounding box is inside the frustum
    bool isBoundingBoxInFrustum(const BoundingBox& bbox) const;
};

