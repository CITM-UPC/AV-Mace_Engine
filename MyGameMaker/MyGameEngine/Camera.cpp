#include "Camera.h"
#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::dmat4 Camera::projection() const {
	return glm::perspective(_fov, _aspect, zNear, zFar);
}

glm::dmat4 Camera::view() const {
	return glm::lookAt(this->owner()->GetComponent<Transform>().pos(), this->owner()->GetComponent<Transform>().pos() + this->owner()->GetComponent<Transform>().fwd(), this->owner()->GetComponent<Transform>().up());
}

std::list<Plane> Camera::frustumPlanes() const
{
    const auto h_fov = _fov;  // FOV horizontal
    const auto v_fov = 2.0 * glm::atan(glm::tan(glm::radians(h_fov) / 2.0) / _aspect);  // Calcular FOV vertical basado en el horizontal y el aspect ratio

    // Posición y orientación de la cámara
    const auto& transform = this->owner()->GetComponent<Transform>();
    const auto& pos = transform.pos();
    const auto& fwd = transform.fwd();
    const auto& up = transform.up();
    const auto& left = transform.left();

    return {
        // Near plane
        Plane(fwd, pos + fwd * zNear),

        // Far plane
        Plane(-fwd, pos + fwd * zFar),

        // Left plane (calculado usando FOV y la relación de aspecto)
        Plane(glm::normalize(glm::cross(up, fwd)), pos),

        // Right plane (calculado usando FOV y la relación de aspecto)
        Plane(glm::normalize(glm::cross(fwd, up)), pos),

        // Top plane
        Plane(glm::normalize(glm::cross(fwd, left)), pos),

        // Bottom plane
        Plane(glm::normalize(glm::cross(left, fwd)), pos)
    };
}
