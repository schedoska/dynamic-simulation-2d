#pragma once

#include "circle_object.h"

#include <SFML/Graphics.hpp>

class CircleDebug : public ds2::circle_object
{
public:
	CircleDebug(vl::vec2d pos, double radius);
	void draw(sf::RenderWindow& window);

private:
	sf::CircleShape _shape;
};
