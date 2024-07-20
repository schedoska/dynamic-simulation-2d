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
	void scale(const vl::vec2d& s) override;

private:
	sf::ConvexShape _shape;
};

