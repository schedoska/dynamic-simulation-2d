#include "object_conf_ui.h"

object_conf_ui::object_conf_ui()
{
	_target_handler = nullptr;
}

void object_conf_ui::set_target(body_handler* target)
{
	_target_handler = target;
}

void object_conf_ui::draw()
{
	// To do: Add throw exception function after you learn how to use it.
	if (_target_handler == nullptr) return;
	body* _target = _target_handler->target();
	if (_target == nullptr) return;

	sf::Color col = _target->color();
	float g[4] = { col.r / 255.f, col.g / 255.f, col.b / 255.f, col.a / 255.f };
	//static float g[4];

	ImGui::Begin(_target->name().c_str());
	
	ImGui::ColorPicker4("MyColor##4", (float*)g, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB);
	col.r = g[0] * 255.f;
	col.g = g[1] * 255.f;
	col.b = g[2] * 255.f;
	col.a = g[3] * 255.f;
	_target->set_color(col);


	ImGui::SeparatorText("Physical properties");

	ImGui::LabelText("Area", (std::to_string(_target->shape().area()) + " m^2").c_str());
	
	static bool adj_inertia = false;
	float target_mass = _target->mass();
	if (ImGui::DragFloat("Mass", &target_mass, 1.0, 0.0, 10e10, "%.2f kg", ImGuiSliderFlags_AlwaysClamp))
	{
		_target->set_mass(target_mass, adj_inertia);
	}
	//ImGui::SameLine();
	ImGui::Checkbox("Adjust inertia", &adj_inertia);

	float target_inertia = _target->inertia();
	if (ImGui::DragFloat("Inertia", &target_inertia, 1.0, 0.0, 10e10, "%.2f kg*m/s^2", ImGuiSliderFlags_AlwaysClamp))
	{
		_target->inertia() = target_inertia;
	}

	ImGui::SeparatorText("Position and rotation");
	float target_x_pos = _target->pos()[0];
	float target_y_pos = _target->pos()[1];
	float target_rot = _target->rot();

	bool hupdt = false;	// handler border update flag
	if (ImGui::DragFloat("X", &target_x_pos, 1)) hupdt = true;
	if (ImGui::DragFloat("Y", &target_y_pos, 1)) hupdt = true;
	if (ImGui::DragFloat("Rotation", &target_rot, 0.02)) hupdt = true;	
	if (hupdt) {
		_target->pos() = vl::vec2d(target_x_pos, target_y_pos);
		_target->rot() = target_rot;
		_target_handler->set_border();
	}

	ImGui::End();
	_target->update_shape();
}

