#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "body.h"
#include "body_handler.h"
#include "polygon_tool.h"
#include "drawable_joint.h"
#include "joint_handler.h"

#include "ui/object_conf_ui.h"
#include "ui/main_tools_ui.h"
#include "ui/simulation_ui.h"
#include "ui/joint_conf_ui.h"

#include "ds2/scene.h"

enum class app_mode {edition, simulation};

struct joint_at_data
{
	drawable_spring* joint;
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

	void add_concave_body(const std::vector<vl::vec2d>& vertices, bool delauney = true);
	void add_convex_body(const std::vector<vl::vec2d>& vertices);
	void add_circle_body(const vl::vec2d& pos, const double& radius);

	void remove(const body* b);
	//void remove(const drawable_spring* ds);
	
	// simulation
	void start_simulation();
	void stop_simulation();
	void set_step_time(const float& step_time);

	void save_json(const std::string& path);
	void load_json(const std::string& path);

private:
	app_mode _mode;
	sf::RenderWindow *_window;

	std::vector<body*> _bodies;
	std::vector<drawable_spring*> _joints;

	ds2::scene _scene;
	float _step_time;

	body_handler bh;
	joint_handler jh;

	polygon_tool pt;
	object_conf_ui oc_ui;
	main_tools_ui mt_ui;
	simulation_ui sim_ui;
	joint_conf_ui jc_ui;

	body* body_at(const vl::vec2d& scene_pos);
	const joint_at_data joint_at(const vl::vec2d& scene_pos) const;
};
