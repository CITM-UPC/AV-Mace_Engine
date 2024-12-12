#pragma once
#include "Transform.h"

class Camera : public Component 
{
	double _fov = glm::radians(60.0);
	double _aspect = 16.0 / 9.0 ;
	double zNear = 0.1;
	double zFar = 1280.0;
	float _speed = 10.0f;

public:
	Camera(bool active, GameObject* owner) : Component(owner) {}
	~Camera() {}

	const double fov() const { return _fov; };
	const double aspect() const { return _aspect; };
	const double near() const { return zNear; };
	const double far() const { return zFar; };
	const float speed() const { return _speed; };
	double& fov() { return _fov; };
	double& aspect() { return _aspect; };
	double& near() { return zNear; }
	double& far() { return zFar; }
	float& speed() { return _speed; };

	void setFov(double fov) { if (fov >= 1.0 && fov <= 120.0) _fov = fov; }
	void setNear(double near) { if (near > 0.0 && near < zFar) zNear = near; }
	void setFar(double far) { if (far > zNear) zFar = far; }

	glm::dmat4 projection() const;
	glm::dmat4 view() const;

	std::list<Plane> frustumPlanes() const;
};


