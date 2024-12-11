#pragma once

#include "Panel.h"
#include <string>

class PanelScene : public Panel
{
public:
	PanelScene(std::string name);
	~PanelScene();

	bool Draw() override;

private:
	void RenderScene();
};

