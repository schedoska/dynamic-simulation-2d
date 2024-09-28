#pragma once

#include <SFML/Graphics.hpp>
#include "ds2/vec.h"

class polygon_tool
{
public:
	polygon_tool();
	void update(const sf::Window* window);
	void draw(sf::RenderWindow* window);
	void start_shape();
	void end();

private:
	bool _active;						// is currently drawing
	std::vector<vl::vec2d> _vertices;	// vertices 
};

