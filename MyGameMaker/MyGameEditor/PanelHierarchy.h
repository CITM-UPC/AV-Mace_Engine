#ifndef __PANEL_HIERARCHY_H__
#define __PANEL_HIERARCHY_H__
#pragma once

#include "Panel.h"
#include <memory>
#include "MyGameEngine/GameObject.h"

class PanelHierarchy : public Panel
{

public:
	PanelHierarchy(std::string name);
	~PanelHierarchy();

	bool Draw();
	void DrawGameObjectTree(GameObject& gameObject);
	GameObject* selectedGameObject() { return _selectedGameObject; }
	void SetSelectedGameObject(GameObject* gameObject) { _selectedGameObject = gameObject; }

private:
	GameObject* _selectedGameObject;
};

#endif // !__PANEL_HIERARCHY_H__