#pragma once

#include <SFML/Graphics.hpp>
#include "vec2.h"

class image
{
	image(const std::string& path);
	void detect_edges();
	double score(vl::vec2d pt, vl::vec2d prev, double radius);

//private:
	sf::Image _img;
};

