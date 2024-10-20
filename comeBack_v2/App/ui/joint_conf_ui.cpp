#include "joint_conf_ui.h"

joint_conf_ui::joint_conf_ui()
{
}

void joint_conf_ui::set_target(dble_joint* target)
{
	_target = target;
	if (!target) return;
	
	if (_jc) {
		delete _jc;
		_jc = nullptr;
	}

	switch (target->joint()->type()) {
	case ds2::joint_type::spring:
		_jc = new spring_joint_conf(dynamic_cast<dble_spring*>(target));
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

spring_joint_conf::spring_joint_conf(dble_spring* target)
{
	_target = target;
}

void spring_joint_conf::draw()
{
	ds2::spring_joint* j = _target->spring_joint();
	ImGui::SeparatorText("Spring joint");

	float strength = j->strength();
	ImGui::DragFloat("strength", &strength);
	j->set_strength(strength);

	float damping = j->damping();
	ImGui::DragFloat("damping", &damping);
	j->set_damping(damping);

	bool auto_len = _target->auto_lenght();
	ImGui::Checkbox("Auto length", &auto_len);
	_target->set_auto_length(auto_len);

	if (auto_len) {
		ImGui::BeginDisabled();
		j->set_length(j->loc_distance());
	}

	float length = j->length();
	ImGui::DragFloat("length", &length);
	j->set_length(length);

	if (auto_len) {
		ImGui::EndDisabled();
	}
}

hinge_joint_conf::hinge_joint_conf(ds2::hinge_joint* target)
{
	_target = target;
}

void hinge_joint_conf::draw()
{
	ImGui::SeparatorText("Hinge joint");

	float stiffness = _target->stiffness();
	ImGui::DragFloat("stiffness", &stiffness);
	_target->set_stiffness(stiffness);

	float firction = _target->friction();
	ImGui::DragFloat("friction", &firction);
	_target->set_friction(firction);
}

motor_joint_conf::motor_joint_conf(ds2::motor_joint* target)
{
	_target = target;
}

void motor_joint_conf::draw()
{
	ImGui::SeparatorText("Motor joint");

	float ang_vel = _target->ang_vel();
	ImGui::DragFloat("target angular vel", &ang_vel);
	_target->set_ang_vel(ang_vel);

	float torque = _target->torque();
	ImGui::DragFloat("torque", &torque);
	_target->set_torque(torque);

	float stiffness = _target->stiffness();
	ImGui::DragFloat("stiffness", &stiffness);
	_target->set_stiffness(stiffness);

	float firction = _target->friction();
	ImGui::DragFloat("friction", &firction);
	_target->set_friction(firction);
}
