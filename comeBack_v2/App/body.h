#pragma once

#include "ds2/object.h"
#include <SFML/Graphics.hpp>

class body : public ds2::object
{
public:
	struct graphics_settings
	{
		graphics_settings(
			bool fill_first, 
			float outline_thicness, 
			sf::Color outline_color,
			bool default_color,
			sf::Color fill_color = sf::Color::White);
		graphics_settings();

		bool fill_first;
		bool default_color;
		float outline_thicness;
		sf::Color outline_color;
		sf::Color fill_color;

		bool display_box;
		float box_thicness;
		sf::Color box_color;
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

	void set_graphics_settings(const graphics_settings& settings);

private:
	graphics_settings _settings;
	sf::Color _fill_color;
	sf::RectangleShape _box_shape;

	std::string _name;
	unsigned int _id;

	std::vector<sf::CircleShape> _circle_gr;
	std::vector<sf::ConvexShape> _convex_gr;

	void draw_outlines(sf::RenderWindow& window);
	void draw_infill(sf::RenderWindow& window);

	void draw_line(
		const vl::vec2d& beg,
		const vl::vec2d& end,
		sf::RenderWindow& window,
		sf::Color color,
		const float& thicness);

	void draw_point(
		const vl::vec2d& pos,
		sf::RenderWindow& window,
		sf::Color color,
		const float& radius
	);

	void update_fill_color();
};

