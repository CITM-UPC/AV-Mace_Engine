#pragma once
#include "GameObject.h"

class Component : public GameObject::IComponent
{
	bool _active;
	GameObject* _owner;

public:
	explicit Component(GameObject* owner) : _active(true), _owner(owner)  {}
	Component(const Component& other) = delete;
	Component(Component&& other) noexcept {}
	virtual ~Component() = default;

	const GameObject* owner() const override { return _owner; }
	GameObject* owner() override { return _owner; }
	void SetOwner(GameObject* owner) override { this->_owner = owner; }

	auto& isActive() { return _active; }
	bool SetActive(bool active) { return this->_active = active; }
	void SwitchState() { _active = !_active; }

	virtual bool operator==(const Component& other) const { return _active == other._active && _owner == other._owner; }
	virtual bool operator!=(const Component& other) const { return !(*this == other); }
};
