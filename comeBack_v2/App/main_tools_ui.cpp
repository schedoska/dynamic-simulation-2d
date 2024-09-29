#include "main_tools_ui.h"

#define _USE_MATH_DEFINES
#include <math.h>

main_tools_ui::main_tools_ui()
{
}

void main_tools_ui::draw()
{
	ImGui::Begin("Tools");


	// Cincave tool invocation creation
	ImGui::SeparatorText("Concave shape");
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.094, 0.216, 0.961, 1));
	ImGui::Button("Add concave");
	ImGui::PopStyleColor(1);
	static bool delauney_tri = true;
	ImGui::Checkbox("Delauney triangulation", &delauney_tri);


	// Convex creation
	ImGui::SeparatorText("Convex shape");
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.027, 0.741, 0.318, 1));
	static int sides = 3;
	if (ImGui::Button("Add convex") && _create_convex_cbck) {
		_create_convex_cbck(create_convex({ 300,300 }, 100, sides));
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
	ImGui::SeparatorText("Convex shape");
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.961, 0.239, 0.298, 1));
	if (ImGui::Button("Add circle") && _create_circle_cbck) {
		_create_circle_cbck({ 100,100 }, 50);
	}
	ImGui::PopStyleColor(1);

	ImGui::End();
}

void main_tools_ui::set_create_convex_cbck(std::function<void(std::vector<vl::vec2d>)> func)
{
	_create_convex_cbck = func;
}

void main_tools_ui::set_create_circle_cbck(std::function<void(vl::vec2d pos, double radius)> func)
{
	_create_circle_cbck = func;
}

void main_tools_ui::set_start_polygon_tool_cbck(std::function<void(void)> func)
{
	_start_polygon_tool_cbck = func;
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
		std::cout << pos + (v * size) << "\n";
	}
	return vertices;
}
