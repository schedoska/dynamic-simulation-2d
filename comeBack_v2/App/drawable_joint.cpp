#include "drawable_joint.h"
#include "ds2/Utils.h"

drawable_spring::drawable_spring(
	ds2::object* a, 
	ds2::object* b, 
	const vl::vec2d& pos_a, 
	const vl::vec2d& pos_b
)
	: ds2::spring(a, b, pos_a, pos_b)
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

void drawable_spring::draw(sf::RenderWindow& window)
{
	vl::vec2d a_pos = global_a();
	vl::vec2d b_pos = global_b();
	utils::drawLine(a_pos, b_pos, window, sf::Color::Magenta);
	
	_a_shape.setPosition(utils::vec2_to_sfml(a_pos));
	_b_shape.setPosition(utils::vec2_to_sfml(b_pos));
	window.draw(_a_shape);
	window.draw(_b_shape);
}
