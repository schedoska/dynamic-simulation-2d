#pragma once

#include "body.h"
#include <SFML/Graphics.hpp>
#include <deque>

class marker
{
public:
	marker();
	void draw(sf::RenderWindow& window);
	void draw_path(sf::RenderWindow& window);
	void update(const sf::Window* window);

	body* target_body();
	void set_body(body* body);
	void set_global_pos(const vl::vec2d& global_pos);
	const vl::vec2d& pos() const;

private:
	std::deque<vl::vec2d> _path;
	size_t _path_length;
	size_t _path_res;
	size_t _path_step;

	body* _target_body;
	vl::vec2d _loc_pos;

	sf::RectangleShape _marker_shape;

	void draw_line(
		const vl::vec2d& beg,
		const vl::vec2d& end,
		sf::RenderWindow& window,
		sf::Color color,
		const float& thicness);
};

