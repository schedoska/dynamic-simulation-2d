#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include "joint_handler.h"

#include <functional>

class joint_conf_ui
{
public:
	joint_conf_ui();
	void set_target(joint_handler* target);
	void draw();

private:
	joint_handler* _target_handler;
};

