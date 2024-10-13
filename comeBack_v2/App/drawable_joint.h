#pragma once

#include "ds2/joint.h"
#include <SFML/Graphics.hpp>

class drawable_joint
{
public:
	virtual void draw(sf::RenderWindow& window) = 0;
};

class dble_spring : public ds2::spring_joint, public drawable_joint
{
public:
	dble_spring(
		ds2::object* a,
		ds2::object* b,
		const vl::vec2d& pos_a = vl::vec2d(),
		const vl::vec2d& pos_b = vl::vec2d());
	void draw(sf::RenderWindow& window) override;

private:
	sf::CircleShape _a_shape;
	sf::CircleShape _b_shape;
};

class dble_hinge : public ds2::hinge_joint, public drawable_joint
{
public:
	dble_hinge(
		ds2::object* a,
		ds2::object* b,
		const vl::vec2d& pos_a = vl::vec2d(),
		const vl::vec2d& pos_b = vl::vec2d());
	void draw(sf::RenderWindow& window) override;

private:
	sf::CircleShape _shape;
};

class dble_motor : public ds2::motor_joint, public drawable_joint
{
public:
	dble_motor(
		ds2::object* a,
		ds2::object* b,
		const vl::vec2d& pos_a = vl::vec2d(),
		const vl::vec2d& pos_b = vl::vec2d());
	void draw(sf::RenderWindow& window) override;

private:
	sf::CircleShape _shape;
};

