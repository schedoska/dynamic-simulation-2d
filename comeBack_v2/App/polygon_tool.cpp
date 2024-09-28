#include "polygon_tool.h"

polygon_tool::polygon_tool()
	: _active(false)
{
}

void polygon_tool::update(const sf::Window* window)
{
	if (!_active) return;

	if ()
}

void polygon_tool::draw(sf::RenderWindow* window)
{
}

void polygon_tool::start_shape()
{
	_active = true;
}

void polygon_tool::end()
{
}
