#pragma once

#include "Panel.h"

class PanelConsole : public Panel
{

public:
	PanelConsole(std::string name);
	~PanelConsole();

	bool Draw() override;
};