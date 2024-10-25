#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "body.h"
#include "dble_joint.h"
#include "grid.h"
#include "marker.h"

#include "tools/body_handler.h"
#include "tools/polygon_tool.h"
#include "tools/joint_handler.h"
#include "tools/force_tool.h"
#include "tools/marker_handler.h"

#include "ui/object_conf_ui.h"
#include "ui/main_tools_ui.h"
#include "ui/simulation_ui.h"
#include "ui/joint_conf_ui.h"
#include "ui/force_tool_ui.h"

#include "ds2/scene.h"

enum class app_mode {edition, simulation};

struct joint_at_data
{
	dble_joint* dble_joint;
	joint_handler_mode type;
};

class app
{
public:
	app(sf::RenderWindow* window);
	~app();

	void update(const sf::Time& dt);
	void simulation_update(const sf::Time& dt);
	void edition_update(const sf::Time& dt);
	void draw();

	void create_body(const ds2::shape_group& shape, const vl::vec2d& pos);
	void create_body_cpy(const body& original, const vl::vec2d& pos);
	void create_joint(ds2::joint_type type, const vl::vec2d& pos_a, const vl::vec2d& pos_b);
	void create_joint_cpy(dble_joint& original, const vl::vec2d& pos_a, const vl::vec2d& pos_b);

	void remove(const body* b);
	void remove(const dble_joint* j);
	void remove_all();
	
	// simulation
	void start_simulation();
	void restart_simulation();
	void set_step_time(const float& step_time);

	void save_json(const std::string& path);
	void load_json(const std::string& path);

	void update_graphics_settings();

private:
	app_mode _mode;
	sf::RenderWindow *_window;

	std::vector<body*> _bodies;
	std::vector<dble_joint*> _dble_joints;
	std::vector<marker*> _markers;

	ds2::scene _scene;
	float _step_time;
	unsigned int _current_id;

	// For copying mechanism
	body* _buffor_body;
	dble_joint* _buffor_joint;

	// Tools
	body_handler bh;
	joint_handler jh;
	polygon_tool pt;
	force_tool ft;
	marker_handler mh;

	// UI elements 
	object_conf_ui oc_ui;
	main_tools_ui mt_ui;
	simulation_ui sim_ui;
	joint_conf_ui jc_ui;
	force_tool_ui ft_ui;
	
	grid _grid;
	body::graphics_settings _graphic_settings;

	body* body_at(const vl::vec2d& scene_pos) const;
	joint_at_data joint_at(const vl::vec2d& scene_pos) const;
	marker* marker_at(const vl::vec2d& scene_pos) const;
};
