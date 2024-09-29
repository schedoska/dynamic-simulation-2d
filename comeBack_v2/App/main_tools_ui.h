#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include "ds2/shape_group.h"

#include <functional>

class main_tools_ui
{
public:
	main_tools_ui();
	void draw();

	void set_create_convex_cbck(std::function<void(std::vector<vl::vec2d>)> func);
	void set_create_circle_cbck(std::function<void(vl::vec2d pos, double radius)> func);
	void set_start_polygon_tool_cbck(std::function<void(void)> func);

private:
	std::function<void(std::vector<vl::vec2d>)> _create_convex_cbck;
	std::function<void(const vl::vec2d& pos, const double& radius)> _create_circle_cbck;
	std::function<void(void)> _start_polygon_tool_cbck;

	std::vector<vl::vec2d> create_convex(const vl::vec2d& pos, const double& size, const int sides);
};

