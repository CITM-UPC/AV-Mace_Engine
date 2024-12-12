#include "Camera.h"
#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

glm::dmat4 Camera::projection() const {
	return glm::perspective(_fov, _aspect, zNear, zFar);
}

glm::dmat4 Camera::view() const {
	return glm::lookAt(this->getOwner()->GetComponent<Transform>()->pos(), this->getOwner()->GetComponent<Transform>()->pos() + this->getOwner()->GetComponent<Transform>()->fwd(), this->getOwner()->GetComponent<Transform>()->up());
}

std::list<Plane> Camera::frustumPlanes() const {
    std::list<Plane> planes;
    const auto h_fov = _fov;  // FOV horizontal
    const auto v_fov = 2.0 * glm::atan(glm::tan(glm::radians(h_fov) / 2.0) / _aspect);  // Calcular FOV vertical basado en el horizontal y el aspect ratio

    // Posición y orientación de la cámara
    const auto& transform = this->getOwner()->GetComponent<Transform>();
    const auto& pos = transform->pos();
    const auto& fwd = transform->fwd();
    const auto& up = transform->up();
    const auto& right = glm::cross(fwd, up);

    // Near plane
    planes.emplace_back(pos + fwd * zNear, fwd);

    // Far plane
    planes.emplace_back(pos + fwd * zFar, -fwd);

    // Top plane
    planes.emplace_back(pos, glm::rotate(fwd, v_fov / 2.0f, right));

    // Bottom plane
    planes.emplace_back(pos, glm::rotate(fwd, -v_fov / 2.0f, right));

    // Left plane
    planes.emplace_back(pos, glm::rotate(fwd, h_fov / 2.0f, up));

    // Right plane
    planes.emplace_back(pos, glm::rotate(fwd, -h_fov / 2.0f, up));

    return planes;
}

