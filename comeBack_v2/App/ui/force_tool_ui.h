#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include "../tools/force_tool.h"

class force_tool_ui
{
public:
	force_tool_ui();
	void draw();
	void set_target(force_tool* ft);

private:
	force_tool* _force_tool;
};

