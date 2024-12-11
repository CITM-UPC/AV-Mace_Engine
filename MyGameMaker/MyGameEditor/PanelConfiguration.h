#pragma once
#include "Panel.h"

#include <vector>

class PanelConfiguration :  public Panel
{
public:
	PanelConfiguration(std::string name);
	~PanelConfiguration();

	bool Draw() override;
};

