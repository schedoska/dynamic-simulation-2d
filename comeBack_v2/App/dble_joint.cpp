#include "dble_joint.h"
#include "ds2/Utils.h"

dble_spring::dble_spring(
	body* a,
	body* b,
	const vl::vec2d& pos_a,
	const vl::vec2d& pos_b
)
	: dble_joint(a, b)
{
	_a_shape.setFillColor(sf::Color::Red);
	_a_shape.setOutlineColor(sf::Color::White);
	_a_shape.setOutlineThickness(3);
	_a_shape.setRadius(10);
	_a_shape.setOrigin(10, 10);

	_b_shape.setFillColor(sf::Color::Red);
	_b_shape.setOutlineColor(sf::Color::White);
	_b_shape.setOutlineThickness(3);
	_b_shape.setRadius(10);
	_b_shape.setOrigin(10, 10);

	_spring_joint = new ds2::spring_joint(a, b, pos_a, pos_b);
}

dble_spring::~dble_spring()
{
	delete _spring_joint;
}

void dble_spring::draw(sf::RenderWindow& window)
{
	vl::vec2d a_pos = _spring_joint->global_a();
	vl::vec2d b_pos = _spring_joint->global_b();
	utils::drawLine(a_pos, b_pos, window, sf::Color::Magenta);
	
	_a_shape.setPosition(utils::vec2_to_sfml(a_pos));
	_b_shape.setPosition(utils::vec2_to_sfml(b_pos));
	window.draw(_a_shape);
	window.draw(_b_shape);
}

ds2::spring_joint* dble_spring::spring_joint()
{
	return _spring_joint;
}

const ds2::spring_joint* dble_spring::spring_joint() const
{
	return _spring_joint;
}

ds2::joint* dble_spring::joint()
{
	return _spring_joint;
}

dble_hinge::dble_hinge(
	body* a,
	body* b,
	const vl::vec2d& pos_a,
	const vl::vec2d& pos_b
)
	: dble_joint(a, b)
{
	_shape.setFillColor(sf::Color::Green);
	_shape.setOutlineColor(sf::Color::White);
	_shape.setOutlineThickness(3);
	_shape.setRadius(10);
	_shape.setOrigin(10, 10);

	_hinge_joint = new ds2::hinge_joint(a, b, pos_a, pos_b);
}

dble_hinge::~dble_hinge()
{
	delete _hinge_joint;
}

void dble_hinge::draw(sf::RenderWindow& window)
{
	// Both a and b gloabl position should be the same
	vl::vec2d pos = _hinge_joint->global_a();
	_shape.setPosition(utils::vec2_to_sfml(pos));
	window.draw(_shape);
}

ds2::hinge_joint* dble_hinge::hinge_joint()
{
	return _hinge_joint;
}

const ds2::hinge_joint* dble_hinge::hinge_joint() const
{
	return _hinge_joint;
}

ds2::joint* dble_hinge::joint()
{
	return _hinge_joint;
}

dble_motor::dble_motor(
	body* a,
	body* b,
	const vl::vec2d& pos_a,
	const vl::vec2d& pos_b
)
	: dble_joint(a, b)
{
	_shape.setFillColor(sf::Color::Blue);
	_shape.setOutlineColor(sf::Color::White);
	_shape.setOutlineThickness(3);
	_shape.setRadius(10);
	_shape.setOrigin(10, 10);

	_motor_joint = new ds2::motor_joint(a, b, 1, pos_a, pos_b);
}

dble_motor::~dble_motor()
{
	delete _motor_joint;
}

void dble_motor::draw(sf::RenderWindow& window)
{
	// Both a and b gloabl position should be the same
	vl::vec2d pos = _motor_joint->global_a();
	_shape.setPosition(utils::vec2_to_sfml(pos));
	window.draw(_shape);
}

ds2::motor_joint* dble_motor::motor_joint()
{
	return _motor_joint;
}

const ds2::motor_joint* dble_motor::motor_joint() const
{
	return _motor_joint;
}

ds2::joint* dble_motor::joint()
{
	return _motor_joint;
}

dble_joint::dble_joint(body* a, body* b)
	:_body_a(a), _body_b(b)
{
}

body* dble_joint::body_a()
{
	return _body_a;
}

body* dble_joint::body_b()
{
	return _body_b;
}

void dble_joint::set_body_a(body* body_a)
{
	_body_a = body_a;
	joint()->set_obj_a(body_a);
}

void dble_joint::set_body_b(body* body_b)
{
	_body_b = body_b;
	joint()->set_obj_b(body_b);
}
