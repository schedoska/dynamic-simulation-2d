#pragma once

#include "ds2/object.h"
#include <SFML/Graphics.hpp>

class body : public ds2::object
{
public:
	enum display_mode {
		off, object_color, white
	};

	body(const unsigned int& id);
	body(
		const unsigned int& id,
		const std::string& name,
		ds2::shape_group shape, 
		const vl::vec2d& pos);
	void draw(sf::RenderWindow& window);
	void update_shape();

	void set_fill_color(const sf::Color& fill_color);
	const sf::Color& fill_color() const;

	void set_name(const std::string& name);
	const std::string& name() const;
	const unsigned int& id() const;
	void set_id(const unsigned int& id);

	void set_body_display_mode(display_mode mode);
	void set_edges_display_mode(display_mode mode);

private:
	float _outline_thicness = 2;
	sf::Color _fill_color;
	std::string _name;
	unsigned int _id;

	std::vector<sf::CircleShape> _circle_gr;
	std::vector<sf::ConvexShape> _convex_gr;
};

