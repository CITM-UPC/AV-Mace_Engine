#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include "Material.h"

GameObject::GameObject(const std::string& name, const std::string& tag, bool active) : _name(name), _tag(tag), _active(active) { this->AddComponent<Transform>(); }
