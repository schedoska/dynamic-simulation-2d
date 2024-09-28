#pragma once

#include "ds2/object.h"
#include <SFML/Graphics.hpp>

class body : public ds2::object
{
public:
	body(const std::string& name);
	void draw(sf::RenderWindow& window);
	void update_shape();

	void set_color(const sf::Color& color);
	const sf::Color& color() const;
	void set_name(const std::string& name);
	const std::string& name() const;

private:
	sf::Color _color;
	std::string _name;

	std::vector<sf::CircleShape> _circle_gr;
	std::vector<sf::ConvexShape> _convex_gr;
};

