#pragma once

#include "ds2/object.h"
#include <SFML/Graphics.hpp>

class body : public ds2::object
{
public:
	body(const unsigned int& id);
	body(
		const unsigned int& id,
		const std::string& name,
		ds2::shape_group shape, 
		const vl::vec2d& pos);
	void draw(sf::RenderWindow& window);
	void update_shape();

	void set_color(const sf::Color& color);
	const sf::Color& color() const;
	void set_name(const std::string& name);
	const std::string& name() const;
	const unsigned int& id() const;
	void set_id(const unsigned int& id);

private:
	sf::Color _color;
	std::string _name;
	unsigned int _id;

	std::vector<sf::CircleShape> _circle_gr;
	std::vector<sf::ConvexShape> _convex_gr;
};

