#include "drawable_joint.h"
#include "ds2/Utils.h"

dble_spring::dble_spring(
	ds2::object* a, 
	ds2::object* b, 
	const vl::vec2d& pos_a, 
	const vl::vec2d& pos_b
)
	: ds2::spring_joint(a, b, pos_a, pos_b)
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
}

void dble_spring::draw(sf::RenderWindow& window)
{
	vl::vec2d a_pos = global_a();
	vl::vec2d b_pos = global_b();
	utils::drawLine(a_pos, b_pos, window, sf::Color::Magenta);
	
	_a_shape.setPosition(utils::vec2_to_sfml(a_pos));
	_b_shape.setPosition(utils::vec2_to_sfml(b_pos));
	window.draw(_a_shape);
	window.draw(_b_shape);
}

dble_hinge::dble_hinge(
	ds2::object* a,
	ds2::object* b,
	const vl::vec2d& pos_a,
	const vl::vec2d& pos_b
)
	: hinge_joint(a, b, pos_a, pos_b)
{
	_shape.setFillColor(sf::Color::Green);
	_shape.setOutlineColor(sf::Color::White);
	_shape.setOutlineThickness(3);
	_shape.setRadius(10);
	_shape.setOrigin(10, 10);
}

void dble_hinge::draw(sf::RenderWindow& window)
{
	// Both a and b gloabl position should be the same
	vl::vec2d pos = global_a();
	_shape.setPosition(utils::vec2_to_sfml(pos));
	window.draw(_shape);
}

dble_motor::dble_motor(
	ds2::object* a, 
	ds2::object* b, 
	const vl::vec2d& pos_a, 
	const vl::vec2d& pos_b
)
	: motor_joint(a, b, 0.2, pos_a, pos_b)
{
	_shape.setFillColor(sf::Color::Blue);
	_shape.setOutlineColor(sf::Color::White);
	_shape.setOutlineThickness(3);
	_shape.setRadius(10);
	_shape.setOrigin(10, 10);
}

void dble_motor::draw(sf::RenderWindow& window)
{
	// Both a and b gloabl position should be the same
	vl::vec2d pos = global_a();
	_shape.setPosition(utils::vec2_to_sfml(pos));
	window.draw(_shape);
}
