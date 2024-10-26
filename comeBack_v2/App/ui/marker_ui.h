#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include "../tools/marker_handler.h"

class marker_ui
{
public:
	marker_ui();
	void set_target(marker_handler* target);
	void draw();

private:
	marker_handler* _target_handler;
};

