#pragma once

#include "ds2/joint.h"
#include "body.h"
#include <SFML/Graphics.hpp>

class dble_joint
{
public:
	dble_joint(body* a, body* b);
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual ~dble_joint() = default;
	virtual dble_joint* create_copy() = 0;

	body* body_a();
	body* body_b();
	void set_body_a(body* body_a);
	void set_body_b(body* body_b);
	virtual ds2::joint* joint() = 0;

private:
	body* _body_a;
	body* _body_b;
};

class dble_spring : public dble_joint
{
public:
	dble_spring(
		body* a,
		body* b,
		const vl::vec2d& pos_a = vl::vec2d(),
		const vl::vec2d& pos_b = vl::vec2d());
	~dble_spring();
	dble_joint* create_copy() override;

	void draw(sf::RenderWindow& window) override;
	ds2::spring_joint* spring_joint();
	const ds2::spring_joint* spring_joint() const;
	ds2::joint* joint() override;

	bool auto_lenght() const;
	void set_auto_length(const bool auto_length);

private:
	sf::CircleShape _a_shape;
	sf::CircleShape _b_shape;
	ds2::spring_joint* _spring_joint;
	bool _auto_length;
};

class dble_hinge : public dble_joint
{
public:
	dble_hinge(
		body* a,
		body* b,
		const vl::vec2d& pos_a = vl::vec2d(),
		const vl::vec2d& pos_b = vl::vec2d());
	~dble_hinge();
	dble_joint* create_copy() override;

	void draw(sf::RenderWindow& window) override;
	ds2::hinge_joint* hinge_joint();
	const ds2::hinge_joint* hinge_joint() const;
	ds2::joint* joint() override;

private:
	sf::CircleShape _shape;
	ds2::hinge_joint* _hinge_joint;
};

class dble_motor : public dble_joint
{
public:
	dble_motor(
		body* a,
		body* b,
		const vl::vec2d& pos_a = vl::vec2d(),
		const vl::vec2d& pos_b = vl::vec2d());
	~dble_motor();
	dble_joint* create_copy() override;

	void draw(sf::RenderWindow& window) override;
	ds2::motor_joint* motor_joint();
	const ds2::motor_joint* motor_joint() const;
	ds2::joint* joint() override;

private:
	sf::CircleShape _shape;
	ds2::motor_joint* _motor_joint;
};

namespace dble_joint_conf 
{
	constexpr double joint_radius = 7.0;
	constexpr double joint_outline_thickness = 2.0;
	
	const sf::Color hinge_color = sf::Color(35, 196, 35);
	const sf::Color motor_color = sf::Color(3, 111, 252);
	const sf::Color spring_color = sf::Color(209, 74, 69);

	const sf::Color joint_outline_color = sf::Color::White;
}
