#pragma once

#include <SFML/Graphics.hpp>
#include "../body.h"

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
	body* target() const;
	const bool is_active() const;

	void set_border();	// adjust border to fit target	

private:
	bool eql_axis_mode;	// used with circles, equal scaling in both axis
	body* _target;
	handler _current_handler;
	bool _active;
	sf::Vector2f _grab_pos;		// realtive to border center

	std::array<sf::CircleShape, 8> _stretchers;
	sf::CircleShape _rotator;
	sf::RectangleShape _border;
	sf::RectangleShape _velocity_line;
	sf::CircleShape _velocity_com;

	void update_handlers_pos();
	void update_active(const sf::Vector2f& mouse_pos);
	void stretch_border(handler h);
	bool borders_contains(const sf::Vector2f& v);
	double snap_angle(const double& rad, const int n);

	void set_target();	// adjust target to fit the border

	/* Draws line visualizating direction and lenght of velocity vector */
	void draw_velocity(sf::RenderWindow* window);
};

namespace handler_conf 
{
	constexpr float handler_radius = 8;
	constexpr float border_width = 4;

	const sf::Color handler_color = sf::Color(235, 235, 235);
	const sf::Color handler_hover_color = sf::Color(95, 161, 227);
	const sf::Color border_color = sf::Color(66, 66, 66);
	const sf::Color border_hover_color = sf::Color(91, 119, 245);

	constexpr float min_border_size = 20;
	constexpr float rotator_top_dist = 40;
	constexpr float border_margin = 20;

	constexpr float velocity_com_radius = 8;
	constexpr float velocity_line_width = 4;
	const sf::Color velocity_line_color = sf::Color(89, 40, 250, 100);
}

