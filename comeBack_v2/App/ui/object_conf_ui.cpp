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

	ImGui::Begin("Object configuration");

	static char target_name[128];
	strcpy_s(target_name, _target->name().c_str());
	if (ImGui::InputText("Name", target_name, IM_ARRAYSIZE(target_name))) {
		_target->set_name(target_name);
	}
	ImGui::Text("Object ID: %d", _target->id());

	draw_color_selector();
	draw_physical_properties();
	draw_position_and_rotation();
	draw_velocity();
	draw_layers();
	draw_other();

	ImGui::End();
	_target->update_shape();
}

void object_conf_ui::draw_color_selector()
{
	body* _target = _target_handler->target();

	sf::Color col = _target->fill_color();
	float g[4] = { col.r / 255.f, col.g / 255.f, col.b / 255.f, col.a / 255.f };
	ImGui::ColorPicker4("MyColor##4", (float*)g, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB);
	col.r = g[0] * 255.f;
	col.g = g[1] * 255.f;
	col.b = g[2] * 255.f;
	col.a = g[3] * 255.f;
	_target->set_fill_color(col);
}

void object_conf_ui::draw_physical_properties()
{
	body* _target = _target_handler->target();
	bool is_static = _target->mass() == ds2::inf_mass ? true : false;

	ImGui::SeparatorText("Physical properties");
	ImGui::LabelText("Area", (std::to_string(_target->shape().area()) + " m^2").c_str());

	if (ImGui::Checkbox("Static", &is_static)) {
		if (is_static) {
			_target->set_mass(ds2::inf_mass, false);
			_target->inertia() = ds2::inf_inertia;
			_target->vel() = vl::vec2d(0, 0);
			_target->rot_vel() = 0;
		}
		else {
			_target->set_mass(1, true);
		}
	}

	if (is_static) ImGui::BeginDisabled();
	static bool adj_inertia = true;
	float target_mass = _target->mass();
	if (ImGui::DragFloat("Mass", &target_mass, 1.0, 0.0, 10e10, "%.2f kg", ImGuiSliderFlags_AlwaysClamp))
	{
		_target->set_mass(target_mass, adj_inertia);
	}

	ImGui::Checkbox("Adjust inertia", &adj_inertia);

	float target_inertia = _target->inertia();
	if (ImGui::DragFloat("Inertia", &target_inertia, 1.0, 0.0, 10e10, "%.2f kg*m/s^2", ImGuiSliderFlags_AlwaysClamp))
	{
		_target->inertia() = target_inertia;
	}
	if (is_static) ImGui::EndDisabled();
}

void object_conf_ui::draw_position_and_rotation()
{
	body* _target = _target_handler->target();

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
}

void object_conf_ui::draw_velocity()
{
	body* _target = _target_handler->target();
	bool is_static = _target->mass() == ds2::inf_mass ? true : false;

	if (is_static) ImGui::BeginDisabled();

	ImGui::SeparatorText("Velocity");
	float target_x_vel = _target->vel()[0];
	float target_y_vel = _target->vel()[1];
	float target_rot_vel = _target->rot_vel();
	ImGui::DragFloat("X vel", &target_x_vel, 1);
	ImGui::DragFloat("Y vel", &target_y_vel, 1);
	ImGui::DragFloat("Rotation vel", &target_rot_vel, 0.02);
	_target->vel() = vl::vec2d(target_x_vel, target_y_vel);
	_target->rot_vel() = target_rot_vel;

	if (is_static) ImGui::EndDisabled();
}

void object_conf_ui::draw_layers()
{
	ImGui::SeparatorText("Layers");
	body* _target = _target_handler->target();

	bool one_layer = (_target->layer_max() == _target->layer_min());
	if (ImGui::Checkbox("One layer", &one_layer)) {
		if (!one_layer) _target->layer_max() = _target->layer_min() + 1;
	}
	if (one_layer) {
		ImGui::InputInt("Layer", &_target->layer_min());
		_target->layer_max() = _target->layer_min();
	}
	else {
		ImGui::InputInt("Layer min", &_target->layer_min());
		ImGui::InputInt("Layer max", &_target->layer_max());
	}
	_target->layer_min() = std::min(_target->layer_min(), _target->layer_max());
}

void object_conf_ui::draw_other()
{
	body* _target = _target_handler->target();

	ImGui::SeparatorText("Other");

	float drag_coeff = _target->drag_coeff();
	ImGui::SliderFloat("Drag coeff", &drag_coeff, 0, 1);
	_target->drag_coeff() = drag_coeff;
}

