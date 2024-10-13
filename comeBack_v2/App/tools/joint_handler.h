#pragma once

#include <SFML/Graphics.hpp>
#include "../ds2/joint.h"
#include "../body.h"

enum class joint_handler_mode { a, b, both };

class joint_handler
{
public:
	joint_handler();
	void update(const sf::Window* window);
	void draw(sf::RenderWindow* window);
	
	void set_target(
		ds2::joint* target, 
		joint_handler_mode mode = joint_handler_mode::a);
	void set_border();
	ds2::joint* target() const;
	const bool is_active() const;
	void target_set_object(ds2::object* obj);

private:
	sf::RectangleShape _border;
	sf::RectangleShape _connected_obj_shape;
	bool _active;
	sf::Vector2f _grab_pos;		// realtive to border center
	
	ds2::joint* _target;
	joint_handler_mode _mode;
	bool _turn;		// true = a assignment turn, false = b

	void set_target();
	void draw_line(
		const vl::vec2d& beg,
		const vl::vec2d& end,
		const sf::Color& color,
		sf::RenderWindow* window);
};

