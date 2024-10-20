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
	_a_shape.setFillColor(dble_joint_conf::spring_color);
	_a_shape.setOutlineColor(dble_joint_conf::joint_outline_color);
	_a_shape.setOutlineThickness(dble_joint_conf::joint_outline_thickness);
	_a_shape.setRadius(dble_joint_conf::joint_radius);
	_a_shape.setOrigin(dble_joint_conf::joint_radius, dble_joint_conf::joint_radius);

	_b_shape.setFillColor(dble_joint_conf::spring_color);
	_b_shape.setOutlineColor(dble_joint_conf::joint_outline_color);
	_b_shape.setOutlineThickness(dble_joint_conf::joint_outline_thickness);
	_b_shape.setRadius(dble_joint_conf::joint_radius);
	_b_shape.setOrigin(dble_joint_conf::joint_radius, dble_joint_conf::joint_radius);

	_auto_length = true;
	_spring_joint = new ds2::spring_joint(a, b, pos_a, pos_b); 
}

dble_spring::~dble_spring()
{
	delete _spring_joint;
}

dble_joint* dble_spring::create_copy()
{
	ds2::spring_joint* js = new ds2::spring_joint(*_spring_joint);
	dble_spring* dble_s = new dble_spring(nullptr, nullptr);
	delete dble_s->_spring_joint;
	dble_s->_spring_joint = js;
	return dble_s;
}

void dble_spring::draw(sf::RenderWindow& window)
{
	vl::vec2d a_pos = _spring_joint->global_a();
	vl::vec2d b_pos = _spring_joint->global_b();
	utils::drawLine(a_pos, b_pos, window, sf::Color::Black);
	
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

bool dble_spring::auto_lenght() const
{
	return _auto_length;
}

void dble_spring::set_auto_length(const bool auto_length)
{
	_auto_length = auto_length;
}

dble_hinge::dble_hinge(
	body* a,
	body* b,
	const vl::vec2d& pos_a,
	const vl::vec2d& pos_b
)
	: dble_joint(a, b)
{
	_shape.setFillColor(dble_joint_conf::hinge_color);
	_shape.setOutlineColor(dble_joint_conf::joint_outline_color);
	_shape.setOutlineThickness(dble_joint_conf::joint_outline_thickness);
	_shape.setRadius(dble_joint_conf::joint_radius);
	_shape.setOrigin(dble_joint_conf::joint_radius, dble_joint_conf::joint_radius);

	_hinge_joint = new ds2::hinge_joint(a, b, pos_a, pos_b);
}

dble_hinge::~dble_hinge()
{
	delete _hinge_joint;
}

dble_joint* dble_hinge::create_copy()
{
	ds2::hinge_joint* js = new ds2::hinge_joint(*_hinge_joint);
	dble_hinge* dble_s = new dble_hinge(nullptr, nullptr);
	delete dble_s->_hinge_joint;
	dble_s->_hinge_joint = js;
	return dble_s;
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
	_shape.setFillColor(dble_joint_conf::motor_color);
	_shape.setOutlineColor(dble_joint_conf::joint_outline_color);
	_shape.setOutlineThickness(dble_joint_conf::joint_outline_thickness);
	_shape.setRadius(dble_joint_conf::joint_radius);
	_shape.setOrigin(dble_joint_conf::joint_radius, dble_joint_conf::joint_radius);

	_motor_joint = new ds2::motor_joint(a, b, pos_a, pos_b);
}

dble_motor::~dble_motor()
{
	delete _motor_joint;
}

dble_joint* dble_motor::create_copy()
{
	ds2::motor_joint* js = new ds2::motor_joint(*_motor_joint);
	dble_motor* dble_s = new dble_motor(nullptr, nullptr);
	delete dble_s->_motor_joint;
	dble_s->_motor_joint = js;
	return dble_s;
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
