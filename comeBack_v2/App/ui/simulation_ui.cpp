#include "simulation_ui.h"

simulation_ui::simulation_ui()
{
	_scene = nullptr;
	_step_time = 25;	// in ms
	_nominal_fps = 40;
	_sim_state = simulation_ui_state::edition;
}

void simulation_ui::set_start_sim_cbck(std::function<void(void)> func)
{
	_start_sim_cbck = func;
}

void simulation_ui::set_restart_sim_cbck(std::function<void(void)> func)
{
	_restart_sim_cbck = func;
}

const float simulation_ui::step_time() const
{
	return _sim_state == simulation_ui_state::paused ? 0 : _step_time;
}

void simulation_ui::draw()
{
	ImGui::Begin("Simulation");

	if (_sim_state == simulation_ui_state::edition) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.027, 0.741, 0.082, 1));
		if (ImGui::Button("Start")) {
			if (_start_sim_cbck) {
				_start_sim_cbck();
				_sim_state = simulation_ui_state::simulation;
			}
		}
		ImGui::PopStyleColor(1);
	}
	else if (_sim_state == simulation_ui_state::paused) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.027, 0.741, 0.082, 1));
		if (ImGui::Button("Start")) {
			_sim_state = simulation_ui_state::simulation;
		}
		ImGui::PopStyleColor(1);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.878, 0.165, 0.247, 1));
		if (ImGui::Button("Stop")) {
			_sim_state = simulation_ui_state::paused;
		}
		ImGui::PopStyleColor(1);
	}
	
	if (_sim_state == simulation_ui_state::paused ||
		_sim_state == simulation_ui_state::simulation) {
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.949, 0.188, 0.188, 1));
		if (ImGui::Button("Restart")) {
			if (_restart_sim_cbck) {
				_restart_sim_cbck();
				_sim_state = simulation_ui_state::edition;
			}
		}
		ImGui::PopStyleColor(1);
	}

	ImGui::Text("FPS: %.2f", _fps);
	ImGui::Text("Object count: %d", _scene->objects().size());

	ImGui::SeparatorText("Simulation speed");

	if (ImGui::InputFloat("Time step", &_step_time, 0.2f, 10.0f, "%.1f ms")) {
		_step_time = std::max(_step_time, 0.f);
	}
	float norm_step = (1.0 / _nominal_fps) * 1000.0;	// in ms
	float speed_ratio = (_step_time / norm_step) * 100.0; // relative to normal speed for selected fps 
	if (ImGui::DragFloat("asd", &speed_ratio, 0.2, 0, 10e10, "%.1f %%")) {
		_step_time = norm_step * speed_ratio / 100.0;
	}

	ImGui::SeparatorText("Joints");
	int iterations = _scene->joint_iterations();
	ImGui::InputInt("Iterations", &iterations, 1, 10);
	iterations = std::max(iterations, 0);
	_scene->set_joint_iterations(iterations);

	ImGui::End();
}

void simulation_ui::set_scene(ds2::scene* scene)
{
	_scene = scene;
}

void simulation_ui::set_fps(const float& fps, const float& nominal_fps)
{
	_fps = fps;
	_nominal_fps = nominal_fps;
}
