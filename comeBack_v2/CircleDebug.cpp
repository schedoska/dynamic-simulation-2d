#include "CircleDebug.h"

CircleDebug::CircleDebug(vl::vec2d pos, double radius)
	: ds2::circle_object(pos, radius)
{
	_shape.setFillColor(sf::Color::Transparent);
	_shape.setOutlineThickness(2);
	_shape.setOutlineColor(sf::Color::Blue);
}

void CircleDebug::draw(sf::RenderWindow& window)
{
	double rad = radius();
	_shape.setRadius(rad);
	_shape.setPosition(pos()[0], pos()[1]);
	_shape.setOrigin(rad, rad);
	window.draw(_shape);
}

