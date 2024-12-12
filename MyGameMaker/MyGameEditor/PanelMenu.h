#pragma once

#include "Panel.h"

enum DrawMode {
	MESH,
	WIREFRAME,
	VERTEX
};

class PanelMenu : public Panel
{

public:
	PanelMenu(std::string name);
	~PanelMenu();

	bool Draw() override;

	void showVertexNormals(auto& child, bool showVertex);
	void showFaceNormals(auto& child, bool showFace);

private:
	int drawMode = 0;
	DrawMode currentDrawMode = DrawMode::MESH;

	bool showPerTriangleNormals = false;
	bool showPerFaceNormals = false;
};