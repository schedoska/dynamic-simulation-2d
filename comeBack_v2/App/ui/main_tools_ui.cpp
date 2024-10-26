#include "main_tools_ui.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "../ds2/regular_shape.h"

main_tools_ui::main_tools_ui()
{
	_grid = nullptr;
}

void main_tools_ui::draw()
{
	ImGui::Begin("Tools");

	ImGui::SeparatorText("Scene");
	if (ImGui::Button("Clear all", ImVec2(ImGui::GetWindowSize().x * 0.9f, 0.0f))
		&& _remove_all_cbck) 
	{
		_remove_all_cbck();
	}

	// Cincave tool invocation creation
	ImGui::SeparatorText("Concave shape");
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.153, 0.306, 0.741, 1));
	if (ImGui::Button("Add concave", ImVec2(ImGui::GetWindowSize().x * 0.9f, 0.0f))) {
		_pt->start_shape();
	}
	ImGui::PopStyleColor(1);

	static int traingulation_mode = 1;
	bool b_del = ImGui::RadioButton("Delauney", &(traingulation_mode), 1);
	bool b_exp = ImGui::RadioButton("Expanding", &(traingulation_mode), 0);
	if (b_del || b_exp)	{
		_pt->set_triangulation_mode(traingulation_mode);
	}
	
	// Convex creation
	ImGui::SeparatorText("Convex shape");
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.153, 0.306, 0.741, 1));
	static int sides = 3;
	if (ImGui::Button("Add convex", ImVec2(ImGui::GetWindowSize().x * 0.9f, 0.0f))
		&& _create_body_cbck) 
	{
		_create_body_cbck(
			generate_regular_shape(sides, main_tools_conf::new_shape_size), 
			main_tools_conf::new_shape_pos);
	}
	ImGui::PopStyleColor(1);

	ImGui::AlignTextToFramePadding();
	ImGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat, true);
	if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { sides--; }
	ImGui::SameLine(0.0f);
	if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { sides++; }

	sides = std::max(std::min(sides, 10), 3);
	ImGui::PopItemFlag();
	ImGui::SameLine();
	ImGui::Text("%d", sides);
	ImGui::SameLine();
	ImGui::Text("sides");
	ImGui::PopStyleColor(1);

	// Circle creation
	ImGui::SeparatorText("Circle shape");
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.153, 0.306, 0.741, 1));

	if (ImGui::Button("Add circle", ImVec2(ImGui::GetWindowSize().x * 0.9f, 0.0f)) && _create_body_cbck) {
		_create_body_cbck(
			generate_circle_shape({ 0,0 }, main_tools_conf::new_shape_size),
			main_tools_conf::new_shape_pos);
	}
	ImGui::PopStyleColor(1);

	// Joints creation
	ImGui::SeparatorText("Joints");
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.678, 0.678, 0.678, 1));
	if (ImGui::Button("Add spring", ImVec2(ImGui::GetWindowSize().x * 0.9f, 0.0f)) && _create_joint_cbck) {
		_create_joint_cbck(
			ds2::joint_type::spring, 
			main_tools_conf::new_joint_pos_a, 
			main_tools_conf::new_joint_pos_b);
	}
	ImGui::PopStyleColor(1);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.722, 0.608, 0, 1));
	if (ImGui::Button("Add hinge", ImVec2(ImGui::GetWindowSize().x * 0.9f, 0.0f)) && _create_joint_cbck) {
		_create_joint_cbck(
			ds2::joint_type::hinge,
			main_tools_conf::new_joint_pos_a,
			main_tools_conf::new_joint_pos_b);
	}
	ImGui::PopStyleColor(1);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.424, 0.6, 0.949, 1));
	if (ImGui::Button("Add motor", ImVec2(ImGui::GetWindowSize().x * 0.9f, 0.0f)) && _create_joint_cbck) {
		_create_joint_cbck(
			ds2::joint_type::motor,
			main_tools_conf::new_joint_pos_a,
			main_tools_conf::new_joint_pos_b);
	}
	ImGui::PopStyleColor(1);

	// Grid section
	ImGui::SeparatorText("Grid");

	bool active_grid = _grid->active();
	if (ImGui::Checkbox("Grid snapping", &active_grid)) {
		_grid->set_active(active_grid);
	}

	if (!active_grid) ImGui::BeginDisabled();

	int size_grid = _grid->grid_size();
	if (ImGui::InputInt("Grid size", &size_grid, 5)) {
		size_grid = std::min(std::max(size_grid, 20), 200);
		_grid->set_grid_size(size_grid);
	}

	if (!active_grid) ImGui::EndDisabled();

	ImGui::SeparatorText("Markers");

	static float path_max_len = 1.0;
	static float path_res = 0.02;
	ImGui::InputFloat("Length", &path_max_len, 0.1, 1.0, "%.2f sec");
	ImGui::InputFloat("Resolution", &path_res, 0.01, 0.1, "%.2f sec");
	ImGui::Value("Node count", int(path_max_len / path_res));

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.478, 0.216, 0.961, 1));
	if (ImGui::Button("Add marker", ImVec2(ImGui::GetWindowSize().x * 0.9f, 0.0f)) && _create_marker_cbck) {
		_create_marker_cbck(main_tools_conf::new_shape_pos, path_max_len, path_res);
	}
	ImGui::PopStyleColor(1);

	ImGui::End();
}

void main_tools_ui::set_create_body_cbck(std::function<void(const ds2::shape_group& shape, const vl::vec2d& pos)> func)
{
	_create_body_cbck = func;
}

void main_tools_ui::set_create_joint_cbck(std::function<void(ds2::joint_type type, const vl::vec2d& pos_a, const vl::vec2d& pos_b)> func)
{
	_create_joint_cbck = func;
}

void main_tools_ui::set_create_marker_cbck(std::function<void(const vl::vec2d& pos, double path_max_len, double path_res)> func)
{
	_create_marker_cbck = func;
}

void main_tools_ui::set_remove_all_cbck(std::function<void(void)> func)
{
	_remove_all_cbck = func;
}

void main_tools_ui::set_polygon_tool(polygon_tool* pt)
{
	_pt = pt;
}

void main_tools_ui::set_grid(grid* _grid)
{
	this->_grid = _grid;
}

std::vector<vl::vec2d> main_tools_ui::create_convex(const vl::vec2d& pos, const double& size, const int sides)
{
	std::vector<vl::vec2d> vertices;
	const double ds = 2.0 * M_PI / (double)sides;
	vl::vec2d v = { 0,-1 };
	if (sides % 2 == 0) {
		v = vl::rotate(v, ds / 2.0);
	}

	for (size_t i = 0; i < sides; ++i) {
		vertices.push_back(pos + (v * size));
		v = vl::rotate(v, ds);
	}
	return vertices;
}

ds2::shape_group main_tools_ui::generate_circle_shape(const vl::vec2d loc_pos, const double& size)
{
	ds2::shape_group sg;
	sg.add(ds2::circle_shape(loc_pos, size));
	return sg;
}

ds2::shape_group main_tools_ui::generate_regular_shape(const unsigned int& sides, const double& size)
{
	ds2::shape_group sg;
	sg.add(ds2::regular_shape(sides, size));
	return sg;
}

