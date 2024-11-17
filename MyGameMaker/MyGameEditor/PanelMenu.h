#ifndef __PANEL_MENU_H__
#define __PANEL_MENU_H__
#pragma once

#include "Panel.h"

enum DrawMode {
	Mesh,
	Wireframe,
	Vertexs
};

class PanelMenu : public Panel
{

public:
	PanelMenu(std::string name);
	~PanelMenu();

	bool Draw();

private:
	int drawMode = 0;
	DrawMode currentDrawMode = Mesh;

	bool showPerTriangleNormals = false;
	bool showPerFaceNormals = false;
};


#endif // !__PANEL_MENU_H__