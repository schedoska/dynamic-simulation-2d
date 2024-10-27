#pragma once

#include <SFML/Graphics.hpp>
#include <list>
#include "ds2/scene.h"

class dble_collisions
{
public:
	dble_collisions(const std::list<ds2::object_collision_data>& ocd);
	void draw(sf::RenderWindow& window);

	void set_active(bool active);
	const bool active() const;

private:
	const std::list<ds2::object_collision_data>& _collision_list;
	bool _active;

	void draw_line(
		const vl::vec2d& beg,
		const vl::vec2d& end,
		sf::RenderWindow& window,
		sf::Color color,
		const float& thicness);

	void draw_point(
		const vl::vec2d& pos,
		sf::RenderWindow& window,
		sf::Color color,
		const float& radius
	);
};

