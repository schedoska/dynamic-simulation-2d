#pragma once

#include <SFML/Graphics.hpp>
#include "ds2/Utils.h"

class grid
{
public:
	grid();
	void update(const sf::Window* window);
	void draw(sf::RenderWindow* window);

	vl::vec2d snap(const vl::vec2d& pos) const;
	sf::Vector2f snap(const sf::Vector2f& pos) const;

	const float& grid_size() const;
	void set_grid_size(const float& size);
	const bool& active() const;
	void set_active(bool active);

private:
	sf::Texture _grid_txt;
	sf::Sprite _grid_sprite;

	void draw_line(
		const sf::Vector2f& beg,
		const sf::Vector2f& end,
		sf::RenderTarget& target,
		sf::Color color,
		const float& thicness);

	void draw_point(
		const sf::Vector2f& pos,
		sf::RenderTarget& target,
		sf::Color color,
		const float& radius
	);

	void create_line_grid();
	void create_dot_grid();

	const int _app_width = 2000;
	const int _app_height = 1500;
	
	float _grid_size;
	bool _active;
};

