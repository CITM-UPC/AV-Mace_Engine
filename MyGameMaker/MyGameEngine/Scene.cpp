#include "Scene.h"
#include "types.h"
#include "Camera.h"


#include <glm/glm.hpp>
#include <memory>
#include <iostream>

#include "Engine.h"
#include "MyWindow.h"
#include "Log.h"
#include "Input.h"
#include "ModelLoader.h"
#include "Mesh.h"
#include "Material.h"
#include "Debug.h"
#include "BoundingBox.h"
#include "Frustum.h"
#include "Ray.h"


//camera movement variables
bool rightMouse = false;
int lastMouseX = -1;
int lastMouseY = -1;
int mouseX = 0;
int mouseY = 0;
float yaw = 0.0f;
float pitch = 0.0f;

//camera orbit variables
bool leftMouse = false;
float horizontalAngle = 0.0f;
float verticalAngle = 0.0f;

//camera zoom variables
float fovModifier = 0;
float zoomValue = 0;
bool isZooming = false;

bool fustrum = false;
bool showconsle = false;

void Scene::Start()
{
	ModelLoader loader;
	// Specify the paths for the FBX file and the custom format file
	std::string fbxFilePath = "Assets/street2.FBX";
	std::string customFilePath = "Library/Models/street.model";

	// Save the FBX file to the custom format
	loader.saveFBXToCustomFormat(fbxFilePath, customFilePath);
	//std::shared_ptr<GameObject> sceneobj = loader.loadFromFile("street");
	std::shared_ptr<GameObject> sceneobj = loader.loadFromFile(fbxFilePath);
	root()->addChild(sceneobj);
	root()->GetComponent<Transform>()->updateGlobalMatrix();
	// Init camera
	InitCamera();
}

void Scene::Update(double& dT)
{
	//camera speed
	if (Engine::Instance().input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		camera()->GetComponent<Camera>()->speed() = 20.0f;
	}
	else {
		camera()->GetComponent<Camera>()->speed() = 10.0f;
	}

	//enale fusion culling
	if (Engine::Instance().input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		fustrum = !fustrum;
		showconsle = true;
	}

	if (showconsle) {
		std::system("cls");
	}

	//camera rotation
	if (Engine::Instance().input->GetMouseButtonDown(3) == KEY_DOWN) {
		rightMouse = true;
		Engine::Instance().input->GetMousePosition(lastMouseX, lastMouseY);
	}
	else if (Engine::Instance().input->GetMouseButtonDown(3) == KEY_UP) {
		rightMouse = false;
	}

	if (rightMouse) 
	{
		//camera movement
		if (Engine::Instance().input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			camera()->GetComponent<Transform>()->translate(vec3(0, 0, camera()->GetComponent<Camera>()->speed() * dT));
		}
		if (Engine::Instance().input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			camera()->GetComponent<Transform>()->translate(vec3(camera()->GetComponent<Camera>()->speed() * dT, 0, 0));
		}
		if (Engine::Instance().input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			camera()->GetComponent<Transform>()->translate(vec3(0, 0, -camera()->GetComponent<Camera>()->speed() * dT));
		}
		if (Engine::Instance().input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			camera()->GetComponent<Transform>()->translate(vec3(-camera()->GetComponent<Camera>()->speed() * dT, 0, 0));
		}

		Engine::Instance().input->GetMousePosition(mouseX, mouseY);

		float dx = (float)(mouseX - lastMouseX);
		float dy = (float)(mouseY - lastMouseY);

		// Ajusta la sensibilidad
		float sensitivity = 0.5f;
		dx *= sensitivity;
		dy *= sensitivity;

		yaw = glm::degrees(atan2(camera()->GetComponent<Transform>()->fwd().z, camera()->GetComponent<Transform>()->fwd().x));
		pitch = glm::degrees(atan2(camera()->GetComponent<Transform>()->fwd().y, glm::sqrt(camera()->GetComponent<Transform>()->fwd().x * camera()->GetComponent<Transform>()->fwd().x + camera()->GetComponent<Transform>()->fwd().z * camera()->GetComponent<Transform>()->fwd().z)));

		yaw += dx;
		pitch -= dy;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		//actualizo el vector forward de la camera
		camera()->GetComponent<Transform>()->setFwd(glm::normalize(direction));
		//uso vec3(0,1,0) porque busco el vector up en coordenadas world!!! igualmente actualizo el vector up(local) de la camera
		camera()->GetComponent<Transform>()->setRigth(glm::normalize(glm::cross(vec3(0, 1, 0), camera()->GetComponent<Transform>()->fwd())));
		camera()->GetComponent<Transform>()->setUp(glm::normalize(glm::cross(camera()->GetComponent<Transform>()->fwd(), camera()->GetComponent<Transform>()->right())));

		Engine::Instance().input->GetMousePosition(lastMouseX, lastMouseY);
	}

	//camera zoom
	Engine::Instance().input->GetMouseWheelMotion(isZooming);
	if (isZooming) {
		Engine::Instance().input->GetMouseWheel((int&)zoomValue);
		if (zoomValue > 0) {
			if (fovModifier > -30.0) {
				fovModifier -= 1.0;
			}
		}
		else {
			if (fovModifier < 0) {
				fovModifier += 1.0;
			}
		}
		camera()->GetComponent<Camera>()->fov() = glm::radians(60 + fovModifier);
	}

	//camera focus
	if (Engine::Instance().input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
		if (selectedGameObject != nullptr) {
			camera()->GetComponent<Transform>()->pos() = selectedGameObject->GetComponent<Transform>()->glob_pos() + vec3(0, 3, 8);
			camera()->GetComponent<Transform>()->setFwd(glm::normalize(selectedGameObject->GetComponent<Transform>()->glob_pos() - camera()->GetComponent<Transform>()->pos()));
			camera()->GetComponent<Transform>()->setRigth(glm::normalize(glm::cross(vec3(0, 1, 0), camera()->GetComponent<Transform>()->fwd())));
			camera()->GetComponent<Transform>()->setUp(glm::normalize(glm::cross(camera()->GetComponent<Transform>()->fwd(), camera()->GetComponent<Transform>()->right())));
		}
		else {
			LOG(LogType::LOG_WARNING, "Select an Object!");
		}
	}

	//camera orbit
	if (Engine::Instance().input->GetMouseButtonDown(1) == KEY_DOWN) {
		leftMouse = true;
		Engine::Instance().input->GetMousePosition(lastMouseX, lastMouseY);
	}
	else if (Engine::Instance().input->GetMouseButtonDown(1) == KEY_UP) {
		leftMouse = false;
	}

	if (Engine::Instance().input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && leftMouse) {
		if (selectedGameObject != nullptr) 
		{
			Engine::Instance().input->GetMousePosition(mouseX, mouseY);
			vec3 targetPos = selectedGameObject->GetComponent<Transform>()->glob_pos();

			// Calcular la distancia y offset inicial entre la c�mara y el objeto
			vec3 offset = camera()->GetComponent<Transform>()->pos() - targetPos;
			float orbitDistance = glm::length(offset);

			// Calcular �ngulos iniciales de la c�mara
			horizontalAngle = atan2(offset.x, offset.z);
			verticalAngle = asin(offset.y / orbitDistance);

			// Sensibilidad del movimiento del rat�n
			float sensitivity = 0.005f;

			float dx = (float)(mouseX - lastMouseX);
			float dy = (float)(mouseY - lastMouseY);

			// Actualizar los �ngulos en base al movimiento del rat�n
			horizontalAngle -= dx * sensitivity;
			verticalAngle += dy * sensitivity;

			// Limitar el �ngulo vertical para no pasar por encima o debajo del objeto
			verticalAngle = glm::clamp(verticalAngle, -glm::half_pi<float>() + 0.1f, glm::half_pi<float>() - 0.1f);

			// Calcular la nueva posici�n de la c�mara en coordenadas esf�ricas
			offset.x = orbitDistance * cos(verticalAngle) * sin(horizontalAngle);
			offset.y = orbitDistance * sin(verticalAngle);
			offset.z = orbitDistance * cos(verticalAngle) * cos(horizontalAngle);

			camera()->GetComponent<Transform>()->pos() = targetPos + offset;

			// Actualizar la direcci�n de la c�mara para que mire al objeto
			camera()->GetComponent<Transform>()->setFwd(glm::normalize(targetPos - camera()->GetComponent<Transform>()->pos()));
			camera()->GetComponent<Transform>()->setRigth(glm::normalize(glm::cross(vec3(0, 1, 0), camera()->GetComponent<Transform>()->fwd())));
			camera()->GetComponent<Transform>()->setUp(glm::normalize(glm::cross(camera()->GetComponent<Transform>()->fwd(), camera()->GetComponent<Transform>()->right())));

			Engine::Instance().input->GetMousePosition(lastMouseX, lastMouseY);
		}
		else {
			LOG(LogType::LOG_WARNING, "Select an Object!");
		}
	}

	if (Engine::Instance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
		int mouseX, mouseY;
		Engine::Instance().input->GetMousePosition(mouseX, mouseY);

		// Screen dimensions
		int screenWidth = Engine::Instance().window->renderWidth();
		int screenHeight = Engine::Instance().window->renderHeight();

		if (mouseX < Engine::Instance().window->renderX() || mouseX >(Engine::Instance().window->renderX() + screenWidth) || mouseY < 20 || mouseY > screenHeight) {
			return;
		}
		Ray ray = CalculatePickingRay(mouseX, mouseY - Engine::Instance().window->renderY(), camera()->GetComponent<Camera>(), screenWidth, screenHeight);

		float closestT = FLT_MAX;
		std::shared_ptr<GameObject> closestObject = nullptr;

		// Start the recursive check from the root object
		for (auto& child : root()->children()) {
			closestObject = CheckIntersectionRecursive(ray, child, closestT, closestObject);
		}

		if (closestObject) {
			selectedGameObject = closestObject.get();
			LOG(LogType::LOG_INFO, ("Selected object: " + selectedGameObject->name()).c_str());
		}
		else {
			selectedGameObject = nullptr;
			LOG(LogType::LOG_INFO, "No object selected.");
		}
	}

}

void Scene::PostUpdate()
{
	
}

void Scene::CleanUp()
{
	/*for (auto& child : children())
	{
		child.get()->CleanUp()
	}*/
}

void Scene::OnSceneChange() {}

bool isAABBInsideFrustum(const BoundingBox& aabb, const std::list<Plane>& planes) {
	for (const auto& plane : planes) {
		// Calcula los puntos m�s cercano y lejano al plano
		vec3 farthest = vec3(
			plane.normal.v.x > 0 ? aabb.max.x : aabb.min.x,
			plane.normal.v.y > 0 ? aabb.max.y : aabb.min.y,
			plane.normal.v.z > 0 ? aabb.max.z : aabb.min.z
		);

		vec3 nearest = vec3(
			plane.normal.v.x > 0 ? aabb.min.x : aabb.max.x,
			plane.normal.v.y > 0 ? aabb.min.y : aabb.max.y,
			plane.normal.v.z > 0 ? aabb.min.z : aabb.max.z
		);

		// Revisar si el AABB est� fuera del frustum
		if (glm::dot(plane.normal.v, farthest - plane.origin.p) < 0) {
			return false; // El AABB est� fuera del frustum
		}
	}
	return true; // Est� dentro del frustum
}

void Scene::Draw(GameObject* sceneroot)
{
	std::list<Plane> frustumPlanes = camera()->GetComponent<Camera>()->frustumPlanes();

	for (auto& child : sceneroot->children())
	{
		if (child.get()->isActive() && child->HasComponent<Mesh>() && child->GetComponent<Mesh>()->isActive()) {
			if (fustrum) {
				BoundingBox aabb = child->getBoundingBox().toAABB(child->GetComponent<Transform>()->mat());

				if (isAABBInsideFrustum(aabb, frustumPlanes)) {
					child->GetComponent<Mesh>()->drawModel(); // Dibujar solo si pasa el culling
					if (showconsle)
						cout << child->name() << " inside frustum, " << endl;
				}
				else {
					if (showconsle)
						cout << child->name() << " outside frustum, " << endl;
				}
			}
			else {
				child->GetComponent<Mesh>()->drawModel();
			}
		}

		if (!child->children().empty()) Draw(child.get());
	}

	drawDebugInfoForGraphicObject(*sceneroot);
	showconsle = false;
}

void Scene::loadGameObjectByPath(const std::string& path)
{
	ModelLoader loader;
	std::shared_ptr<GameObject> scene = loader.loadFromFile(path);
	root()->addChild(scene);
	scene->GetComponent<Transform>()->updateGlobalMatrix();

	LOG(LogType::LOG_INFO, "Model loaded successfully!");
}

void Scene::loadTextureByPath(const std::string& path)
{
	if (selectedGameObject != nullptr) {
		selectedGameObject->GetComponent<Material>()->m_Texture = std::make_unique<Texture>(path);
	}
	else {
		LOG(LogType::LOG_WARNING,"Select an Object!");
	}
}

std::shared_ptr<GameObject> Scene::CreateGameObject(const std::string& name)
{
	ModelLoader modelLoader;
	std::shared_ptr<Model> model;
	modelLoader.load(Shapes::EMPTY, model);
	std::shared_ptr<GameObject> go = std::make_shared<GameObject>(name.c_str());
	go->GetComponent<Transform>()->pos() = vec3(0, 0, 0);

	if (selectedGameObject == nullptr) root()->addChild(go);
	else selectedGameObject->addChild(go);

	return go;
}

void Scene::InitCamera()
{
	_camera.AddComponent<Camera>();
	BoundingBox bbox;
	for (auto& child : root()->children().front()->children()) {
		if (child->HasComponent<Mesh>()) {
			bbox = child->getBoundingBox() + bbox;
		}
	}
	_camera.GetComponent<Transform>()->pos() = bbox.center() + vec3(0, 50, bbox.size().z * 1.2);
	_camera.GetComponent<Transform>()->rotate(glm::radians(180.0), vec3(0, 1, 0));
	_camera.GetComponent<Transform>()->rotate(glm::radians(30.0), vec3(1, 0, 0));

	std::shared_ptr<GameObject> camera = std::make_shared<GameObject>(_camera);
	root()->addChild(camera);
}

void Scene::CreateCube()
{
	ModelLoader modelLoader;
	std::shared_ptr<Model> model;
	modelLoader.load(Shapes::CUBE, model);
	std::shared_ptr<GameObject> go = std::make_shared<GameObject>(model.get()->GetMeshName());
	
	if (selectedGameObject == nullptr) root()->addChild(go);
	else selectedGameObject->addChild(go);
	
	go->GetComponent<Transform>()->pos() = vec3(10, 0, 0);
	go->GetComponent<Transform>()->updateGlobalMatrix();
	go->AddComponent<Mesh>();
	go->GetComponent<Mesh>()->setModel(model);
	go->GetComponent<Mesh>()->loadToOpenGL();
	go->AddComponent<Material>();
	go->GetComponent<Material>()->m_Shader = std::make_unique<Shader>("Assets/Shaders/Basic.shader");
}

void Scene::CreatePlane() 
{
	ModelLoader modelLoader;
	std::shared_ptr<Model> model;
	modelLoader.load(Shapes::PLANE, model);
	std::shared_ptr<GameObject> go = std::make_shared<GameObject>(model.get()->GetMeshName());
	go->GetComponent<Transform>()->pos() = vec3(-5, 0, 0);
	go->GetComponent<Transform>()->updateGlobalMatrix();
	go->AddComponent<Mesh>();
	go->GetComponent<Mesh>()->setModel(model);
	go->GetComponent<Mesh>()->loadToOpenGL();
	go->AddComponent<Material>();
	go->GetComponent<Material>()->m_Shader = std::make_unique<Shader>("Assets/Shaders/Basic.shader");

	if (selectedGameObject == nullptr) root()->addChild(go);
	else selectedGameObject->addChild(go);
}

void Scene::CreateSphere()
{
	ModelLoader modelLoader;
	std::shared_ptr<Model> model;
	modelLoader.load(Shapes::SPHERE, model);
	std::shared_ptr<GameObject> go = std::make_shared<GameObject>(model.get()->GetMeshName());
	go->GetComponent<Transform>()->pos() = vec3(10, 0, -5);
	go->GetComponent<Transform>()->updateGlobalMatrix();
	go->AddComponent<Mesh>();
	go->GetComponent<Mesh>()->setModel(model);
	go->GetComponent<Mesh>()->loadToOpenGL();
	go->AddComponent<Material>();
	go->GetComponent<Material>()->m_Shader = std::make_unique<Shader>("Assets/Shaders/Basic.shader");

	if (selectedGameObject == nullptr) root()->addChild(go);
	else selectedGameObject->addChild(go);
}

void Scene::CreateCylinder()
{
	ModelLoader modelLoader;
	std::shared_ptr<Model> model;
	modelLoader.load(Shapes::CYLINDER, model);
	std::shared_ptr<GameObject> go = std::make_shared<GameObject>(model.get()->GetMeshName());
	go->GetComponent<Transform>()->pos() = vec3(-5, 0, -5);
	go->GetComponent<Transform>()->updateGlobalMatrix();
	go->AddComponent<Mesh>();
	go->GetComponent<Mesh>()->setModel(model);
	go->GetComponent<Mesh>()->loadToOpenGL();
	go->AddComponent<Material>();
	go->GetComponent<Material>()->m_Shader = std::make_unique<Shader>("Assets/Shaders/Basic.shader");

	if (selectedGameObject == nullptr) root()->addChild(go);
	else selectedGameObject->addChild(go);
}

void Scene::CreateCone()
{
	ModelLoader modelLoader;
	std::shared_ptr<Model> model;
	modelLoader.load(Shapes::CONE, model);
	std::shared_ptr<GameObject> go = std::make_shared<GameObject>(model.get()->GetMeshName());
	go->GetComponent<Transform>()->pos() = vec3(0, 0, -5);
	go->GetComponent<Transform>()->updateGlobalMatrix();
	go->AddComponent<Mesh>();
	go->GetComponent<Mesh>()->setModel(model);
	go->GetComponent<Mesh>()->loadToOpenGL();
	go->AddComponent<Material>();
	go->GetComponent<Material>()->m_Shader = std::make_unique<Shader>("Assets/Shaders/Basic.shader");

	if (selectedGameObject == nullptr) root()->addChild(go);
	else selectedGameObject->addChild(go);
}

void Scene::CreateTorus()
{
	ModelLoader modelLoader;
	std::shared_ptr<Model> model;
	modelLoader.load(Shapes::TORUS, model);
	std::shared_ptr<GameObject> go = std::make_shared<GameObject>(model.get()->GetMeshName());
	go->GetComponent<Transform>()->pos() = vec3(5, 0, -5);
	go->GetComponent<Transform>()->updateGlobalMatrix();
	go->AddComponent<Mesh>();
	go->GetComponent<Mesh>()->setModel(model);
	go->GetComponent<Mesh>()->loadToOpenGL();
	go->AddComponent<Material>();
	go->GetComponent<Material>()->m_Shader = std::make_unique<Shader>("Assets/Shaders/Basic.shader");

	if (selectedGameObject == nullptr) root()->addChild(go);
	else selectedGameObject->addChild(go);
}

void Scene::CreateCamera()
{
	std::shared_ptr<GameObject> camera = std::make_shared<GameObject>("Camera", "Camera");
	camera->GetComponent<Transform>()->pos() = vec3(0, 0, 0);
	camera->GetComponent<Transform>()->updateGlobalMatrix();
	camera->AddComponent<Camera>();
	camera->GetComponent<Camera>()->fov() = glm::radians(60.0f);
	camera->GetComponent<Camera>()->near() = 0.1f;
	camera->GetComponent<Camera>()->far() = 1000.0f;

	if (selectedGameObject == nullptr) root()->addChild(camera);
	else selectedGameObject->addChild(camera);
}

Ray Scene::CalculatePickingRay(int mouseX, int mouseY, Camera* camera, int screenWidth, int screenHeight) {
	glm::vec4 viewport(WINDOW_WIDTH * 0.15, 200, screenWidth, screenHeight);

	glm::mat4 view = glm::mat4(camera->view());
	glm::mat4 projection = glm::mat4(camera->projection()); 

	glm::vec3 nearPoint = glm::unProject(glm::vec3(mouseX, screenHeight - mouseY, 0.0f),
		view,
		projection,
		viewport);
	glm::vec3 farPoint = glm::unProject(glm::vec3(mouseX, screenHeight - mouseY, 1.0f),
		view,
		projection,
		viewport);

	glm::vec3 direction = glm::normalize(farPoint - nearPoint);
	return Ray{ nearPoint, direction };
}

bool Scene::IntersectRayBox(const Ray& ray, const BoundingBox& box, float& t) {
	float tmin = (box.min.x - ray.origin.x) / ray.direction.x;
	float tmax = (box.max.x - ray.origin.x) / ray.direction.x;

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (box.min.y - ray.origin.y) / ray.direction.y;
	float tymax = (box.max.y - ray.origin.y) / ray.direction.y;

	if (tymin > tymax) std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax)) return false;

	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;

	float tzmin = (box.min.z - ray.origin.z) / ray.direction.z;
	float tzmax = (box.max.z - ray.origin.z) / ray.direction.z;

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax)) return false;

	t = tmin;
	return true;
}


std::shared_ptr<GameObject>& Scene::CheckIntersectionRecursive(const Ray& ray, std::shared_ptr<GameObject> object, float& closestT, std::shared_ptr<GameObject>& closestObject) {
	// Check the bounding box of the current object
	BoundingBox bbox = object->getBoundingBox();
	float t;
	if (object->HasComponent<Mesh>()) {
		if (IntersectRayBox(ray, bbox, t) && t < closestT) {
			closestT = t;
			closestObject = object;
		}
	}

	// Recursively check the children of the current object
	for (auto& child : object->children()) {
		CheckIntersectionRecursive(ray, child, closestT, closestObject);
	}

	return closestObject;
}
