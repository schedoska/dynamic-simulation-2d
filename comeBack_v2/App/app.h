#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "body.h"
#include "body_handler.h"
#include "object_conf_ui.h"
#include "polygon_tool.h"
#include "main_tools_ui.h"
#include "simulation_ui.h"
#include "drawable_joint.h"
#include "joint_handler.h"
#include "joint_conf_ui.h"

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
	
	// simulation
	void start_simulation();

private:
	app_mode _mode;
	sf::RenderWindow *_window;

	std::vector<body*> _bodies;
	std::vector<drawable_spring*> _joints;

	ds2::scene _scene;

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
