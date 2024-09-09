#pragma once

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "body.h"
#include "body_handler.h"

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
};

