#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include "../body_handler.h"

#include <functional>

class object_conf_ui
{
public:
	object_conf_ui();
	void set_target(body_handler* target);
	void draw();

private:
	body_handler* _target_handler;
};

