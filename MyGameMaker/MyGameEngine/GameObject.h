#pragma once

#include <string>
#include <vector>
#include "TreeExtension.h"
#include "PolymorphList.h"

class GameObject : public TreeExtension<GameObject>
{
public:
	class IComponent
	{
	public:
		virtual const GameObject* owner() const = 0;
		virtual GameObject* owner() = 0;
		virtual void SetOwner(GameObject* owner) = 0;
	};

private:
	std::string _name;
	std::string _tag;
	bool _active;
	PolymorphList<IComponent> _components;

public:
	GameObject() {}
	GameObject(const std::string& name, const std::string& tag = "Untagged", bool active = true);
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) noexcept;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) noexcept = delete;
	virtual ~GameObject() = default;

	auto& name() { return _name; }
	auto& tag() { return _tag; }
	
	auto& isActive() { return _active; }
	virtual bool SetActive(bool active) { return this->_active = active; }
	virtual bool SwitchState() { return _active = !_active; }

	template <typename TComponent>
	bool HasComponent() const { return _components.hasType<TComponent>(); }

	template <typename TComponent>
	TComponent& AddComponent() { return _components.emplace<TComponent>(this); }

	template <typename TComponent>
	TComponent& GetComponent() { return _components.getType<TComponent>(); }

	template <typename TComponent>
	const TComponent& GetComponent() const { return _components.getType<TComponent>(); }

	bool operator==(const GameObject& other) const { return _name == other._name && _tag == other._tag && _active == other._active && _components.size() == other._components.size(); }

protected:
	virtual void Start() {}
	virtual void Update() {}
	virtual void CleanUp() {}
	virtual void OnSceneChange() {}
};