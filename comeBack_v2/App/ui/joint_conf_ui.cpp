#include "joint_conf_ui.h"

joint_conf_ui::joint_conf_ui()
{
}

void joint_conf_ui::set_target(dble_joint* target)
{
	_target = target;
	if (!target) return;
	
	if (_jc) delete _jc;

	switch (target->joint()->type()) {
	case ds2::joint_type::spring:
		_jc = new spring_joint_conf(dynamic_cast<ds2::spring_joint*>(target->joint()));
		break;
	case ds2::joint_type::hinge:
		_jc = new hinge_joint_conf(dynamic_cast<ds2::hinge_joint*>(target->joint()));
		break;
	case ds2::joint_type::motor:
		_jc = new motor_joint_conf(dynamic_cast<ds2::motor_joint*>(target->joint()));
		break;
	}
}

void joint_conf_ui::draw()
{
	if (!_target) return;

	ImGui::Begin("Joint");

	auto obj_status = [](body* b, const char* txt)
	{
		ImGui::Text(txt);
		ImGui::SameLine();
		if (b) {
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
			ImGui::Text("SET - %s", b->name());
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::Text("None");
		}
		ImGui::PopStyleColor(1);
	};

	obj_status(_target->body_a(), "Object A:");
	obj_status(_target->body_b(), "Object B:");

	if (!_target->body_a() || !_target->body_b()) {
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		ImGui::Text("Joint not connected!");
		ImGui::PopStyleColor(1);
	}
	if (_jc) _jc->draw();

	ImGui::End();
}

spring_joint_conf::spring_joint_conf(ds2::spring_joint* target)
{
	_target = target;
}

void spring_joint_conf::draw()
{
	ImGui::SeparatorText("Spring joint");
}

hinge_joint_conf::hinge_joint_conf(ds2::hinge_joint* target)
{
	_target = target;
}

void hinge_joint_conf::draw()
{
	ImGui::SeparatorText("Hinge joint");
}

motor_joint_conf::motor_joint_conf(ds2::motor_joint* target)
{
	_target = target;
}

void motor_joint_conf::draw()
{
	ImGui::SeparatorText("Motor joint");
}
