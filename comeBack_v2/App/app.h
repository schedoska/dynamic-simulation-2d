#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "body.h"
#include "body_handler.h"
#include "object_conf_ui.h"
#include "polygon_tool.h"
#include "main_tools_ui.h"

class app
{
public:
	app(sf::RenderWindow* window);
	~app();

	void update(const sf::Time& dt);
	void draw();

	void add_concave_body(const std::vector<vl::vec2d>& vertices, bool delauney = true);
	void add_convex_body(const std::vector<vl::vec2d>& vertices);
	void add_circle_body(const vl::vec2d& pos, const double& radius);

private:
	sf::RenderWindow *_window;
	std::vector<body*> _bodies;

	body_handler bh;
	polygon_tool pt;
	object_conf_ui oc_ui;
	main_tools_ui mt_ui;

	body* body_at(const vl::vec2d& scene_pos);
};

