#pragma once

#include <SFML/Graphics.hpp>
#include "../marker.h"
#include "../grid.h"

class marker_handler
{
public:
	marker_handler();
	void update(const sf::Window* window);
	void draw(sf::RenderWindow* window);

	void set_target(marker* target);
	void set_border();
	void set_grid(grid* g);
	const bool is_active() const;
	void target_set_object(body* obj);
	marker* target() const;

private:
	void set_target();

	marker* _target;
	grid* _grid;

	sf::RectangleShape _border;
	sf::RectangleShape _connected_obj_shape;
	bool _active;
	sf::Vector2f _grab_pos;		// realtive to border center
};

