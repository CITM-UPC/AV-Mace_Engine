#ifndef __PANEL_CONSOLE_H__
#define __PANEL_CONSOLE_H__
#pragma once

#include "Panel.h"

class PanelConsole : public Panel
{

public:
	PanelConsole(std::string name);
	~PanelConsole();

	bool Draw() override;
};

#endif // !__PANEL_CONSOLE_H__