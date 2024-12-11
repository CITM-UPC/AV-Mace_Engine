#include "Frustum.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

std::array<glm::vec3, 8> Frustum::calculateFrustumCorners(const Camera& camera) {
    glm::mat4 viewProj = camera.projection() * camera.view();
    glm::mat4 invViewProj = glm::inverse(viewProj);

    // Clip space corners
    std::array<glm::vec4, 8> clipSpaceCorners = {
        glm::vec4(-1, -1, -1, 1), glm::vec4(1, -1, -1, 1),
        glm::vec4(1, 1, -1, 1),   glm::vec4(-1, 1, -1, 1),
        glm::vec4(-1, -1, 1, 1),  glm::vec4(1, -1, 1, 1),
        glm::vec4(1, 1, 1, 1),    glm::vec4(-1, 1, 1, 1),
    };

    // Transform to world space
    std::array<glm::vec3, 8> worldSpaceCorners;
    for (size_t i = 0; i < 8; ++i) {
        glm::vec4 corner = invViewProj * clipSpaceCorners[i];
        corner /= corner.w;  // Perspective divide
        worldSpaceCorners[i] = glm::vec3(corner);
    }
    return worldSpaceCorners;
}

