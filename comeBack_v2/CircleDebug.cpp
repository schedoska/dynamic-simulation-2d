#include "CircleDebug.h"

CircleDebug::CircleDebug(vl::vec2d pos, double radius)
	: ds2::circle_object(pos, radius)
{
	_shape.setFillColor(sf::Color::Transparent);
	_shape.setOutlineThickness(2);
	_shape.setOutlineColor(sf::Color::Blue);
	_shape.setRadius(radius);
	_shape.setPosition(pos.x, pos.y);
}

void CircleDebug::draw(sf::RenderWindow& window)
{
	window.draw(_shape);
}
