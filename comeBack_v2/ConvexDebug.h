#pragma once
#include "convex_object.h"
//#include "Utils.h"

#include <SFML/Graphics.hpp>

class ConvexDebug : public ds2::convex_object
{
public:
	ConvexDebug(vl::vec2d pos);
	void draw(sf::RenderWindow& window);
	void update_shape();

private:
	sf::ConvexShape _shape;
};

