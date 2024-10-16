#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include "../ds2/shape_group.h"

#include <functional>

class main_tools_ui
{
public:
	main_tools_ui();
	void draw();

	void set_start_polygon_tool_cbck(std::function<void(void)> func);
	void set_create_body_cbck(std::function<void(const ds2::shape_group& shape, const vl::vec2d& pos)> func);

private:
	std::function<void(void)> _start_polygon_tool_cbck;
	std::function<void(const ds2::shape_group& shape, const vl::vec2d& pos)> _create_body_cbck;

	std::vector<vl::vec2d> create_convex(const vl::vec2d& pos, const double& size, const int sides);
	ds2::shape_group generate_circle_shape(const vl::vec2d loc_pos, const double& size);
	ds2::shape_group generate_regular_shape(const unsigned int& sides, const double& size);
};

namespace main_tools_conf
{
	const vl::vec2d new_shape_pos = { 400,400 };
	constexpr float new_shape_size = 100;
}
