#include "Camera.h"
#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::dmat4 Camera::projection() const {
	return glm::perspective(_fov, _aspect, zNear, zFar);
}

glm::dmat4 Camera::view() const {
	return glm::lookAt(this->getOwner()->GetComponent<Transform>()->pos(), this->getOwner()->GetComponent<Transform>()->pos() + this->getOwner()->GetComponent<Transform>()->fwd(), this->getOwner()->GetComponent<Transform>()->up());
}

std::list<Plane> Camera::frustumPlanes() const
{
	const auto h_fov = _fov;
	const auto v_fov = _fov / _aspect;
	return {
		//near
		Plane(this->getOwner()->GetComponent<Transform>()->fwd(), this->getOwner()->GetComponent<Transform>()->pos() + this->getOwner()->GetComponent<Transform>()->fwd() * zNear),
		//far
		Plane(-this->getOwner()->GetComponent<Transform>()->fwd(), this->getOwner()->GetComponent<Transform>()->pos() + this->getOwner()->GetComponent<Transform>()->fwd() * zFar),
		//left
		Plane(glm::rotate(-this->getOwner()->GetComponent<Transform>()->left(), h_fov, this->getOwner()->GetComponent<Transform>()->up()), this->getOwner()->GetComponent<Transform>()->pos()),
		////right
		Plane(glm::rotate(this->getOwner()->GetComponent<Transform>()->left(), -h_fov, this->getOwner()->GetComponent<Transform>()->up()), this->getOwner()->GetComponent<Transform>()->pos()),
		////top
		Plane(glm::rotate(-this->getOwner()->GetComponent<Transform>()->up(), -v_fov, this->getOwner()->GetComponent<Transform>()->left()), this->getOwner()->GetComponent<Transform>()->pos()),
		////bottom
		Plane(glm::rotate(this->getOwner()->GetComponent<Transform>()->up(), v_fov, this->getOwner()->GetComponent<Transform>()->left()), this->getOwner()->GetComponent<Transform>()->pos())
	};
}
