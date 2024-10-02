#pragma once

#include "ds2/joint.h"
#include <SFML/Graphics.hpp>

class drawable_spring : public ds2::spring
{
public:
	drawable_spring(
		ds2::object* a,
		ds2::object* b,
		const vl::vec2d& pos_a = vl::vec2d(),
		const vl::vec2d& pos_b = vl::vec2d());
	void draw(sf::RenderWindow& window);

private:
	sf::CircleShape _a_shape;
	sf::CircleShape _b_shape;
};

