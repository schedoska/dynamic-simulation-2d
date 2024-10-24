#include "force_tool_ui.h"

force_tool_ui::force_tool_ui()
{
	_force_tool = nullptr;
}

void force_tool_ui::draw()
{
	if (!_force_tool) return;

	ImGui::Begin("Force tool");

	bool proportional_force = _force_tool->proportional_force();
	ImGui::Checkbox("Proportional to mass", &proportional_force);
	_force_tool->set_proportional_force(proportional_force);

	float strength = _force_tool->strength();
	if (ImGui::DragFloat("Strength", &strength, 1, 0, 0, "%.1f")) {
		_force_tool->set_strength(strength);
	}
	float damping = _force_tool->damping();
	if (ImGui::DragFloat("Damping", &damping, 0.01, 0, 0, "%.1f")) {
		_force_tool->set_damping(damping);
	}
	float rot_damping = _force_tool->rot_damping();
	if (ImGui::DragFloat("Rotational damping", &rot_damping, 0.01, 0, 0, "%.1f")) {
		_force_tool->set_rot_damping(rot_damping);
	}

	ImGui::End();
}

void force_tool_ui::set_target(force_tool* ft)
{
	_force_tool = ft;
}
