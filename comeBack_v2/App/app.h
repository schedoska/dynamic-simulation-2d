#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "body.h"
#include "body_handler.h"
#include "object_conf_ui.h"

class app
{
public:
	app(sf::RenderWindow* window);
	~app();

	void update(const sf::Time& dt);
	void draw();

private:
	sf::RenderWindow *_window;
	std::vector<body*> _bodies;

	body* body_at(const vl::vec2d& scene_pos);

	body_handler bh;
	object_conf_ui oc_ui;
};

