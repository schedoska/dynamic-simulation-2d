#include "ConvexDebug.h"
#include "Utils.h"

ConvexDebug::ConvexDebug(vl::vec2d pos) :
	convex_object(pos)
{
}

void ConvexDebug::draw(sf::RenderWindow& window)
{
	_shape.setPosition(utils::vec2_to_sfml(pos()));
	_shape.setRotation(utils::RadToDegrees(rot()));
	window.draw(_shape);
}

void ConvexDebug::update_shape()
{
	_shape.setFillColor(sf::Color::Transparent);
	_shape.setOutlineThickness(2);
	_shape.setOutlineColor(sf::Color::Blue);
	_shape.setPointCount(vertices().size());
	auto& vl = vertices();
	for (int i = 0; i < vertices().size(); i++) {
		_shape.setPoint(i, utils::vec2_to_sfml(vl[i]));
	}
}
