#pragma once

#include "GameObject.h"
#include <string>

class GameObject;

class Component
{
	bool _active;
	GameObject* _owner;

public:
	Component() : _active(true), _owner(nullptr) {}
	Component(bool active, GameObject* owner) : _active(active), _owner(owner)  {}
	virtual ~Component() {
		_owner = nullptr; // Remove association with the owner.
		_active = false;  // Mark as inactive to indicate cleanup.
	}

	GameObject* getOwner() const { return _owner; }
	auto& isActive() { return _active; }
	bool SetActive(bool active) { return this->_active = active; }
	void SwitchState() { _active = !_active; }

	virtual bool operator==(const Component& other) const {	return _active == other._active && _owner == other._owner; }
	virtual bool operator!=(const Component& other) const {	return !(*this == other); }
};
