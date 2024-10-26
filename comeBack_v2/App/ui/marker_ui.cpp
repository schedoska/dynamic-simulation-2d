#include "marker_ui.h"

marker_ui::marker_ui()
{
	_target_handler = nullptr;
}

void marker_ui::set_target(marker_handler* target)
{
	_target_handler = target;
}

void marker_ui::draw()
{
	if (_target_handler == nullptr) return;
	marker* _target = _target_handler->target();
	if (_target == nullptr) return;

	ImGui::Begin("Marker");

	ImGui::SameLine();
	if (_target->target_body()) {
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
		ImGui::Text("SET - %s", _target->target_body()->name());
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::Text("None");
	}
	ImGui::PopStyleColor(1);

	float path_max_len = _target->path_max_len();
	float path_res = _target->path_res();
	ImGui::InputFloat("Length", &path_max_len, 0.1, 1.0, "%.2f sec");
	ImGui::InputFloat("Resolution", &path_res, 0.01, 0.1, "%.2f sec");
	ImGui::Value("Node count", int(path_max_len / path_res));
	_target->set_max_path_len(path_max_len);
	_target->set_path_res(path_res);


	sf::Color col = _target->path_color();
	float g[4] = { col.r / 255.f, col.g / 255.f, col.b / 255.f, col.a / 255.f };
	ImGui::ColorPicker4("MyColor##4", (float*)g, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB);
	col.r = g[0] * 255.f;
	col.g = g[1] * 255.f;
	col.b = g[2] * 255.f;
	col.a = g[3] * 255.f;
	_target->set_path_color(col);

	ImGui::End();
}
