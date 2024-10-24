#pragma once

#include <SFML/Graphics.hpp>
#include "../body.h"

class force_tool
{
public:
	force_tool();
	void update(const sf::Window* window, const sf::Time& dt);
	void draw(sf::RenderWindow* window);

	void set_target(body* target, const sf::Vector2i& mouse_pos = sf::Vector2i());
	body* target() const;

	void set_strength(const float& strength);
	void set_damping(const float& damping);
	void set_rot_damping(const float& rot_damping);
	void set_proportional_force(const bool proportional_force);
	const float& strength() const;
	const float& damping() const;
	const float& rot_damping() const;
	bool proportional_force() const;

private:
	body* _target;
	vl::vec2d _loc_grab_pos;	/* In object local coord system */
	vl::vec2d _glob_end_pos;	/* In global coord system */

	sf::RectangleShape _force_line;

	float _strength;
	float _damping;
	float _rot_damping;
	bool _proportional_froce;
};

