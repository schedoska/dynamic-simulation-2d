#include "main_tools_ui.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "../ds2/regular_shape.h"

main_tools_ui::main_tools_ui()
{
}

void main_tools_ui::draw()
{
	ImGui::Begin("Tools");


	// Cincave tool invocation creation
	ImGui::SeparatorText("Concave shape");
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.153, 0.306, 0.741, 1));
	ImGui::Button("Add concave");
	ImGui::PopStyleColor(1);
	static bool delauney_tri = true;
	ImGui::Checkbox("Delauney triangulation", &delauney_tri);


	// Convex creation
	ImGui::SeparatorText("Convex shape");
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.153, 0.306, 0.741, 1));
	static int sides = 3;
	if (ImGui::Button("Add convex") && _create_body_cbck) {
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

	if (ImGui::Button("Add circle") && _create_body_cbck) {
		_create_body_cbck(
			generate_circle_shape({ 0,0 }, main_tools_conf::new_shape_size),
			main_tools_conf::new_shape_pos);
	}
	ImGui::PopStyleColor(1);


	// Joints creation
	ImGui::SeparatorText("Joints");
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.678, 0.678, 0.678, 1));
	if (ImGui::Button("Add spring") && _create_joint_cbck) {
		_create_joint_cbck(
			ds2::joint_type::spring, 
			main_tools_conf::new_joint_pos_a, 
			main_tools_conf::new_joint_pos_b);
	}
	ImGui::PopStyleColor(1);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.722, 0.608, 0, 1));
	if (ImGui::Button("Add hinge") && _create_joint_cbck) {
		_create_joint_cbck(
			ds2::joint_type::hinge,
			main_tools_conf::new_joint_pos_a,
			main_tools_conf::new_joint_pos_b);
	}
	ImGui::PopStyleColor(1);




	ImGui::End();
}

void main_tools_ui::set_start_polygon_tool_cbck(std::function<void(void)> func)
{
	_start_polygon_tool_cbck = func;
}

void main_tools_ui::set_create_body_cbck(std::function<void(const ds2::shape_group& shape, const vl::vec2d& pos)> func)
{
	_create_body_cbck = func;
}

void main_tools_ui::set_create_joint_cbck(std::function<void(ds2::joint_type type, const vl::vec2d& pos_a, const vl::vec2d& pos_b)> func)
{
	_create_joint_cbck = func;
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

