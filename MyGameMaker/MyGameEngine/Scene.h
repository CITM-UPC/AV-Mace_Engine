#pragma once

#include <string>
#include <memory>

#include "GameObject.h"
#include "Camera.h"
#include "Ray.h"

class Scene
{
	std::string _name;
	bool _active;
	GameObject* _root = new GameObject("SceneRoot");
	GameObject _camera = GameObject("MainCamera", "Camera");

public:
	Scene(const std::string& name, bool active = true) : _name(name), _active(active) {}
	~Scene() {}

    GameObject* root() { return _root; }
	GameObject* camera() { return &_camera; }

	void Start();
	void Update(double& dT);
	void PostUpdate();
	void CleanUp();
	void OnSceneChange();
	void Draw(GameObject& sceneroot);

	void loadGameObjectByPath(const std::string& path);
	void loadTextureByPath(const std::string& path);

	GameObject* selectedGameObject = nullptr;

	void InitCamera();
	GameObject& CreateGameObject(const std::string& name = "GameObject");
	GameObject& CreateCube();
	GameObject& CreatePlane();
	GameObject& CreateSphere();
	GameObject& CreateCylinder();
	GameObject& CreateCone();
	GameObject& CreateTorus();
	GameObject& CreateCamera();

	Ray CalculatePickingRay(int mouseX, int mouseY, Camera* camera, int screenWidth, int screenHeight);
	bool IntersectRayBox(const Ray& ray, const BoundingBox& box, float& t);
	GameObject* CheckIntersectionRecursive(const Ray& ray, GameObject& object, float& closestT, GameObject* closestObject) {
};