#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include "../ds2/shape_group.h"
#include "../ds2/joint.h"
#include "../tools/polygon_tool.h"
#include "../grid.h"

#include <functional>

class main_tools_ui
{
public:
	main_tools_ui();
	void draw();

	void set_create_body_cbck(
		std::function<void(const ds2::shape_group& shape, const vl::vec2d& pos)> func);
	void set_create_joint_cbck(
		std::function<void(ds2::joint_type type, const vl::vec2d& pos_a, const vl::vec2d& pos_b)> func);
	void set_create_marker_cbck(std::function<void(const vl::vec2d& pos, double path_max_len, double path_res)> func);
	void set_remove_all_cbck(std::function<void(void)> func);
	void set_polygon_tool(polygon_tool* pt);
	void set_grid(grid* _grid);

private:
	std::function<void(const ds2::shape_group& shape, const vl::vec2d& pos)> _create_body_cbck;
	std::function<void(ds2::joint_type type, const vl::vec2d& pos_a, const vl::vec2d& pos_b)> _create_joint_cbck;
	std::function<void(void)> _remove_all_cbck;
	std::function<void(const vl::vec2d& pos, double path_max_len, double path_res)> _create_marker_cbck;

	std::vector<vl::vec2d> create_convex(const vl::vec2d& pos, const double& size, const int sides);
	ds2::shape_group generate_circle_shape(const vl::vec2d loc_pos, const double& size);
	ds2::shape_group generate_regular_shape(const unsigned int& sides, const double& size);

	polygon_tool* _pt;
	grid* _grid;
};

namespace main_tools_conf
{
	const vl::vec2d new_shape_pos = { 400,400 };
	constexpr float new_shape_size = 100;
	const vl::vec2d new_joint_pos_a = { 400,400 };
	const vl::vec2d new_joint_pos_b = { 500,400 };
}
