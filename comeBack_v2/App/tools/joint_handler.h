#pragma once

#include <SFML/Graphics.hpp>
#include "../dble_joint.h"
#include "../body.h"
#include "../grid.h"

enum class joint_handler_mode { a, b, both };

class joint_handler
{
public:
	joint_handler();
	void update(const sf::Window* window);
	void draw(sf::RenderWindow* window);
	
	void set_target(
		dble_joint* target, 
		joint_handler_mode mode = joint_handler_mode::a);
	void set_border();
	dble_joint* target() const;
	const bool is_active() const;
	void target_set_object(body* obj);
	void set_grid(grid* g);

private:
	sf::RectangleShape _border;
	sf::RectangleShape _connected_obj_shape;
	bool _active;
	sf::Vector2f _grab_pos;		// realtive to border center
	
	dble_joint* _target;
	joint_handler_mode _mode;
	bool _turn;		// true = a assignment turn, false = b
	grid* _grid;

	void set_target();
	void draw_line(
		const vl::vec2d& beg,
		const vl::vec2d& end,
		const sf::Color& color,
		sf::RenderWindow* window);
};

