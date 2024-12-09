#pragma once

#include "GameObject.h"

class Component : public GameObject::IComponent
{
	bool _active;
	GameObject& _owner;

public:
	Component() : _active(true), _owner(*(new GameObject())) {}
	explicit Component(GameObject& owner) : _active(true), _owner(owner)  {}
	virtual ~Component() = default;

	const GameObject& owner() const override 
	{ 
		return _owner; 
	}
	GameObject& owner() override { return _owner; }

	auto& isActive() { return _active; }
	bool SetActive(bool active) { return this->_active = active; }
	void SwitchState() { _active = !_active; }

	virtual bool operator==(const Component& other) const {	return _active == other._active && _owner == other._owner; }
	virtual bool operator!=(const Component& other) const {	return !(*this == other); }
};
