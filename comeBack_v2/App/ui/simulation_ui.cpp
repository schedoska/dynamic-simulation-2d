#include "simulation_ui.h"

simulation_ui::simulation_ui()
{
	_scene = nullptr;
	_target_step_time = 25;	// in ms
	_nominal_fps = 40;
	_sim_state = simulation_ui_state::edition;
	_ips = 1;
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
	return _sim_state == simulation_ui_state::paused ? 0 : _real_step_time;
}

const int simulation_ui::ips() const
{
	return _ips;
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

	float tbf = 1000.0 / _nominal_fps; // time between frames in ms
	if (ImGui::InputFloat("Target time step", &_target_step_time, 0.2f, 10.0f, "%.1f ms")) {
		_target_step_time = std::min(std::max(_target_step_time, 0.f), tbf);
	}
	
	static float _play_speed = 100.0; // in percent
	ImGui::DragFloat("Play speed", &_play_speed, 1, 0, 1000, "%.1f");

	static int performance_mode = 0;
	ImGui::RadioButton("Performace step", &performance_mode, 0);
	ImGui::RadioButton("Accuracy step", &performance_mode, 1);

	// the time that should be simulated between frames in ms
	float _expected_sim_step = (_play_speed / 100.f) * tbf; 

	_ips = int(_expected_sim_step / _target_step_time);
	if (performance_mode) ++_ips;
	
	_real_step_time = _expected_sim_step / float(_ips);
	
	if (ImGui::CollapsingHeader("Simulation speed details", ImGuiTreeNodeFlags_None)) {
		ImGui::Text("Target simulation step: %.1f ms", _expected_sim_step);
		ImGui::Text("Iterations per frame: %d", _ips);
		ImGui::Text("Real time step: %.1f ms", _real_step_time);
		ImGui::Text("Total simulation step: %.1f ms", _real_step_time * _ips);
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
