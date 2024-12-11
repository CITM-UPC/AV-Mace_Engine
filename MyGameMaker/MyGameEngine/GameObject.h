#pragma once

#include <string>
#include <vector>
#include "TreeExtension.h"
#include "Component.h"
#include "BoundingBox.h"

class Component;

class GameObject : public TreeExtension<GameObject>
{
	std::string _name;
	std::string _tag;
	bool _active;
	std::vector<Component*> _components;

	BoundingBox _boundingBox;

public:
	GameObject(const std::string& name, const std::string& tag = "Untagged", bool active = true);
	~GameObject() { Delete(); }

	auto& name() { return _name; }
	auto& tag() { return _tag; }
	
	auto& isActive() { return _active; }
	virtual bool SetActive(bool active) { return this->_active = active; }
	virtual bool SwitchState() { return _active = !_active; }

	template <typename T>
	T* AddComponent() {
		static_assert(std::is_base_of<Component, T>::value, "ERROR: T must inherit from Component");
		T* newComponent = new T(true, this);
		if (newComponent) _components.push_back(dynamic_cast<Component*>(newComponent));
		return newComponent;
	}

	template <typename T>
	T* GetComponent() const {
		for (Component* component : _components) {
			if (T* specificComponent = dynamic_cast<T*>(component)) {
				return specificComponent;
			}
		}
		return nullptr;
	}

	template <typename T>
	bool HasComponent() const {
		for (Component* component : _components) {
			if (dynamic_cast<T*>(component)) {
				return true;
			}
		}
		return false;
	}

	void Delete() {
		_active = false;
		//for (auto& component : _components); //delete component;
		//_components.clear();
		for (auto& child : children()) child->Delete();
		children().clear();
		unparent();
	}

	bool operator==(const GameObject& other) const;
	bool operator!=(const GameObject& other) const;

	// Métodos para la Bounding Box
	const BoundingBox& getBoundingBox() const;
	void setBoundingBox(const BoundingBox& bbox) { _boundingBox = bbox; }

protected:
	virtual void Start() {}
	virtual void Update() {}
	virtual void CleanUp() {}
	virtual void OnSceneChange() {}
};