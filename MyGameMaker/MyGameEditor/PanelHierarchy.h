#pragma once

#include "Panel.h"
#include <memory>
#include "MyGameEngine/GameObject.h"

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy(std::string name);
	~PanelHierarchy();

	bool Draw() override;
	void DrawGameObjectTree(GameObject& gameObject);
	GameObject* selectedGameObject() { return _selectedGameObject; }
	void SetSelectedGameObject(GameObject* gameObject) { _selectedGameObject = gameObject; }

private:
	GameObject* _selectedGameObject = nullptr;
};