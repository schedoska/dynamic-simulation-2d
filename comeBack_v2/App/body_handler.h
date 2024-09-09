#pragma once

#include <SFML/Graphics.hpp>
#include "body.h"

enum handler_type {
	top_border,
	right_border,
	bottom_border,
	left_border,
	top_left_corner,
	top_right_corner,
	bottom_right_corner,
	bottom_left_corner
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
	ds2::rect _box;
	bool _is_stretching;
	handler_type _handler;

	std::array<sf::CircleShape, 4> _corners;
	std::array<sf::RectangleShape, 4> _borders;
	void update_corner_border_pos();
	void update_corner_border_input(const sf::Window* window);
	void stretch_box(const sf::Window* window);
};

namespace body_handler_conf 
{
	constexpr float corner_radius = 4;
	constexpr float border_width = 3;

	const sf::Color corner_color = sf::Color::Green;
	const sf::Color corner_hover_color = sf::Color(2, 2, 2);
	const sf::Color border_color = sf::Color::Red;
	const sf::Color border_hover_color = sf::Color(2, 2, 2);

	constexpr float min_body_size = 20;
}

