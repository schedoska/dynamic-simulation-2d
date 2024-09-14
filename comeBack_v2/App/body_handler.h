#pragma once

#include <SFML/Graphics.hpp>
#include "body.h"

enum handler {
	top,
	right,
	bottom,
	left,
	top_left,
	top_right,
	bottom_right,
	bottom_left,
	rotator,
	position
};

class body_handler
{
public:
	body_handler();
	void update(const sf::Window* window);
	void draw(sf::RenderWindow* window);

	void set_target(body* target);

private:
	body* _target;

	handler _current_handler;
	bool _active;
	sf::Vector2f _grab_pos;		// realtive to border center

	std::array<sf::CircleShape, 8> _stretchers;
	sf::CircleShape _rotator;
	sf::RectangleShape _border;

	void update_handlers_pos();
	void update_active(const sf::Vector2f& mouse_pos);
	void stretch_border(handler h);
	bool borders_contains(const sf::Vector2f& v);
	double snap_angle(const double& rad, const int n);

	void set_target();			// adjust target to fit the border
};

namespace body_handler_conf 
{
	constexpr float corner_radius = 4;
	constexpr float border_width = 4;
	constexpr float center_radius = 6;

	const sf::Color corner_color = sf::Color::Green;
	const sf::Color corner_hover_color = sf::Color(2, 2, 2);
	const sf::Color border_color = sf::Color::Red;
	const sf::Color border_hover_color = sf::Color(2, 2, 2);
	const sf::Color center_color = sf::Color::Red;
	const sf::Color center_hover_color = sf::Color(2, 2, 2);

	constexpr float min_body_size = 20;
}

