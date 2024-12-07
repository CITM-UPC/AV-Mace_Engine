#include "Frustum.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Helper function to create a plane from the view-projection matrix
Plane createPlaneFromMatrix(const mat4& matrix, int row) {
    // Extract the row corresponding to the plane coefficients
    vec4 rowVec = matrix[row];
    vec3 normal = vec3(rowVec.x, rowVec.y, rowVec.z);
    float d = rowVec.w;

    // Assuming the plane passes through the origin
    return Plane(Point(vec3(0.0f)), Vector(normal));
}

void Frustum::setFrustumPlanes(const mat4& viewProjMatrix) {
    // Extract the six planes from the view-projection matrix
    planes[0] = createPlaneFromMatrix(viewProjMatrix, 0); // Left plane
    planes[1] = createPlaneFromMatrix(viewProjMatrix, 1); // Right plane
    planes[2] = createPlaneFromMatrix(viewProjMatrix, 2); // Top plane
    planes[3] = createPlaneFromMatrix(viewProjMatrix, 3); // Bottom plane
    planes[4] = createPlaneFromMatrix(viewProjMatrix, 4); // Near plane
    planes[5] = createPlaneFromMatrix(viewProjMatrix, 5); // Far plane
}

bool Frustum::isBoundingBoxInFrustum(const BoundingBox& bbox) const {
    for (const auto& plane : planes) {
        bool allOutside = true;
        for (const auto& vertex : bbox.vertices()) {
            float distance = glm::dot(plane.normal.v, vertex - plane.origin.p);
            if (distance >= 0) {
                // If any vertex is in front of or on the plane, the box is not entirely outside
                allOutside = false;
                break;
            }
        }
        if (allOutside) {
            // If all vertices are outside the plane, the bounding box is culled
            return false;
        }
    }
    // If the bounding box is not culled by any plane, it's potentially visible
    return true;
}