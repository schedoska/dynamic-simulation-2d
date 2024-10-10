#include "joint_conf_ui.h"

joint_conf_ui::joint_conf_ui()
{
}

void joint_conf_ui::set_target(joint_handler* target)
{
	_target_handler = target;
}

void joint_conf_ui::draw()
{
	ds2::joint* t = _target_handler->target();
	if (!t) return;

	ImGui::Begin("Joint");

	auto obj_status = [](bool active, const char* txt)
	{
		ImGui::Text(txt);
		ImGui::SameLine();
		if (active) {
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
			ImGui::Text("SET");
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::Text("None");
		}
		ImGui::PopStyleColor(1);
	};

	obj_status(t->obj_a(), "Object A:");
	obj_status(t->obj_b(), "Object B:");

	if (!t->obj_a() || !t->obj_b()) {
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::Text("Joint not connected!");
		ImGui::PopStyleColor(1);
	}




	ImGui::End();
}
