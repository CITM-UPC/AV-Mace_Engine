#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include "Material.h"

GameObject::GameObject(const std::string& name, const std::string& tag, bool active) : _name(name), _tag(tag), _active(active) 
{ 
	this->AddComponent<Transform>();
}

GameObject::GameObject(GameObject&& other) noexcept : TreeExtension(std::move(other)), _name(std::move(other._name)), _tag(std::move(other._tag)), _active(other._active), _components(std::move(other._components))
{
	for (auto& component : _components)
	{
		component->SetOwner(this);
	}
}
