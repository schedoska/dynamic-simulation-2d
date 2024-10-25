#include "simulation_ui.h"

simulation_ui::simulation_ui()
{
	_scene = nullptr;
	_target_step_time = 25;	// in ms
	_nominal_fps = 40;
	_sim_state = simulation_ui_state::edition;
	_ips = 1;
	_gravity_v = { 0,100 };
	_settings = nullptr;
}

void simulation_ui::set_start_sim_cbck(std::function<void(void)> func)
{
	_start_sim_cbck = func;
}

void simulation_ui::set_restart_sim_cbck(std::function<void(void)> func)
{
	_restart_sim_cbck = func;
}

void simulation_ui::set_graphic_settings_cbck(std::function<void(void)> func)
{
	_graphic_settings_cbck = func;
}

const float simulation_ui::step_time() const
{
	return _sim_state == simulation_ui_state::paused ? 0 : _real_step_time;
}

const int simulation_ui::ips() const
{
	return _ips;
}

const vl::vec2d& simulation_ui::gravity_v() const
{
	return vl::vec2d(
		static_cast<double>(_gravity_v[0]), 
		static_cast<double>(_gravity_v[1]));
}

void simulation_ui::draw_graphic_ui()
{
	ImGui::SeparatorText("Graphics settings");

	/* Settings for: Show structure = 1 && Color == 1 */
	static body::graphics_settings gs1 = { true, 2.0, sf::Color::White, true };
	/* Settings for: Show structure = 0 && Color == 1 */
	static body::graphics_settings gs2 = { false, 4.0, sf::Color::White, true };
	/* Settings for: Show structure = 1 && Color == 0 */
	static body::graphics_settings gs3 = { true, 2.0, sf::Color::White, false, sf::Color::Transparent };
	/* Settings for: Show structure = 0 && Color == 0 */
	static body::graphics_settings gs4 = { true, 2.0, sf::Color::White, false, sf::Color::Transparent };

	bool settings_changed = false;
	static bool structures = false;
	static bool colors = true;
	if (ImGui::Checkbox("Show structure", &structures)) {
		settings_changed = true;
	}
	if (ImGui::Checkbox("Color", &colors)) {
		settings_changed = true;
	}
	
	if (settings_changed && _graphic_settings_cbck) {
		if (structures && colors) *_settings = gs1;
		else if (!structures && colors) *_settings = gs2;
		else if (structures && !colors) *_settings = gs3;
		else if (!structures && !colors) *_settings = gs4;
		_graphic_settings_cbck();
	}
}

void simulation_ui::draw()
{
	ImGui::Begin("Simulation");

	if (_sim_state == simulation_ui_state::edition) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.027, 0.741, 0.082, 1));
		if (ImGui::Button("Start", ImVec2(ImGui::GetWindowSize().x * 0.4f, 0.0f))) {
			if (_start_sim_cbck) {
				_start_sim_cbck();
				_sim_state = simulation_ui_state::simulation;
			}
		}
		ImGui::PopStyleColor(1);
	}
	else if (_sim_state == simulation_ui_state::paused) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.027, 0.741, 0.082, 1));
		if (ImGui::Button("Start", ImVec2(ImGui::GetWindowSize().x * 0.4f, 0.0f))) {
			_sim_state = simulation_ui_state::simulation;
		}
		ImGui::PopStyleColor(1);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.878, 0.165, 0.247, 1));
		if (ImGui::Button("Stop", ImVec2(ImGui::GetWindowSize().x * 0.4f, 0.0f))) {
			_sim_state = simulation_ui_state::paused;
		}
		ImGui::PopStyleColor(1);
	}
	
	if (_sim_state == simulation_ui_state::paused ||
		_sim_state == simulation_ui_state::simulation) {
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.949, 0.188, 0.188, 1));
		if (ImGui::Button("Restart", ImVec2(ImGui::GetWindowSize().x * 0.4f, 0.0f))) {
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
	ImGui::SliderFloat("Target time step", &_target_step_time, 0, tbf, "%.1f ms");

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
		ImGui::Bullet(); ImGui::Value("Target simulation step", _expected_sim_step, "%.1f ms");
		ImGui::Bullet(); ImGui::Value("Iterations per frame", _ips);
		ImGui::Bullet(); ImGui::Value("Real time step", _real_step_time, "%.1f ms");
		ImGui::Bullet(); ImGui::Value("Total simulation step", _real_step_time * _ips, "%.1f ms");
	}

	ImGui::SeparatorText("Joints");
	int iterations = _scene->joint_iterations();
	ImGui::InputInt("Iterations", &iterations, 1, 10);
	iterations = std::max(iterations, 0);
	_scene->set_joint_iterations(iterations);

	draw_graphic_ui();

	ImGui::SeparatorText("Gravity force");
	static bool no_gravity = false;
	ImGui::Checkbox("No gravity", &no_gravity);

	if (no_gravity) {
		ImGui::BeginDisabled();
		_gravity_v = { 0,0 };
	}
	ImGui::DragFloat("X", &_gravity_v[0]);
	ImGui::DragFloat("Y", &_gravity_v[1]);
	if (no_gravity) ImGui::EndDisabled();


	ImGui::End();
}

void simulation_ui::set_scene(ds2::scene* scene)
{
	_scene = scene;
}

void simulation_ui::set_graphic_settings(body::graphics_settings* settings)
{
	_settings = settings;
}

void simulation_ui::set_fps(const float& fps, const float& nominal_fps)
{
	_fps = fps;
	_nominal_fps = nominal_fps;
}
