#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include "../ds2/shape_group.h"
#include "../ds2/scene.h"
#include "../body.h"

#include <functional>

enum class simulation_ui_state {
	simulation, edition, paused
};

class simulation_ui
{
public:
	simulation_ui();
	void set_scene(ds2::scene* scene);
	void set_graphic_settings(body::graphics_settings* settings);
	void set_fps(const float& fps, const float& nominal_fps);
	void draw();

	void set_start_sim_cbck(std::function<void(void)> func);
	void set_restart_sim_cbck(std::function<void(void)> func);
	void set_graphic_settings_cbck(std::function<void(void)> func);

	const float step_time() const;	// in ms
	const int ips() const;
	const vl::vec2d& gravity_v() const;

private:
	ds2::scene* _scene;
	body::graphics_settings* _settings;
	
	std::function<void(void)> _start_sim_cbck;
	std::function<void(void)> _restart_sim_cbck;
	std::function<void(void)> _graphic_settings_cbck;

	float _fps;
	float _nominal_fps;
	float _target_step_time;	// in ms
	float _real_step_time;		// in ms

	vl::vec2f _gravity_v;
	
	int _ips;	// iterations per step
	simulation_ui_state _sim_state;

	void draw_graphic_ui();
};

