#pragma once

#include "object.h"
#include <SFML/Graphics.hpp>

class DebugObject : public ds2::object
{
public:
	DebugObject(vl::vec2d pos);
	void draw(sf::RenderWindow& window);
	void update_shape();

private:
	std::vector<sf::CircleShape> _circle_gr;
	std::vector<sf::ConvexShape> _convex_gr;
};

