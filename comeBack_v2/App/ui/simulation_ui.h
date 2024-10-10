#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include "../ds2/shape_group.h"
#include "../ds2/scene.h"

#include <functional>

class simulation_ui
{
public:
	simulation_ui();
	void set_scene(ds2::scene* scene);
	void set_fps(const float& fps, const float& nominal_fps);
	void set_sim_on(const bool sim_on);
	void draw();

	void set_start_sim_cbck(std::function<void(void)> func);
	void set_stop_sim_cbck(std::function<void(void)> func);
	void set_restart_sim_cbck(std::function<void(void)> func);

	const float step_time() const;	// in ms

private:
	ds2::scene* _scene;
	
	std::function<void(void)> _start_sim_cbck;
	std::function<void(void)> _stop_sim_cbck;
	std::function<void(void)> _restart_sim_cbck;

	float _fps;
	float _nominal_fps;
	float _step_time;	// in ms
	bool _sim_on;	// true = simulation mode, false = edition mode
};

