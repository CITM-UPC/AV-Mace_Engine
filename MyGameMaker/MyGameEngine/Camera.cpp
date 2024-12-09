#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

glm::dmat4 Camera::projection() const {
    return glm::perspective(_fov, _aspect, zNear, zFar);
}

glm::dmat4 Camera::view() const {
    auto& transform = owner()->GetComponent<Transform>();
    return glm::lookAt(transform.pos(), transform.pos() + transform.fwd(), transform.up());
}