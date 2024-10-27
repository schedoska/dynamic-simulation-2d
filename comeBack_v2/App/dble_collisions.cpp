#include "dble_collisions.h"
#include "ds2/Utils.h"
#include <iostream>

dble_collisions::dble_collisions(const std::list<ds2::object_collision_data>& ocd)
	: _collision_list(ocd)
{

}

void dble_collisions::draw(sf::RenderWindow& window)
{
	if (!_active) return;

	for (const auto& it : _collision_list) {
		ds2::collision_data cd = it.data;
		vl::vec2d dv = (cd.cp_a - cd.cp_b).normalize();
		dv *= 30.0;
		draw_line(cd.cp_a + dv, cd.cp_b - dv, window, sf::Color(4, 219, 187), 5);
		draw_point(cd.cp_a, window, sf::Color(2, 69, 171), 6);
		draw_point(cd.cp_b, window, sf::Color(2, 69, 171), 6);
	}
}

void dble_collisions::set_active(bool active)
{
	_active = active;
}

const bool dble_collisions::active() const
{
	return _active;
}

void dble_collisions::draw_line(
	const vl::vec2d& beg, 
	const vl::vec2d& end, 
	sf::RenderWindow& window, 
	sf::Color color, 
	const float& thicness)
{
	static sf::RectangleShape line;
	line.setPosition(utils::vec2_to_sfml(beg));
	vl::vec2d delta = beg - end;

	line.setSize(sf::Vector2f(delta.len(), thicness));
	float angle = utils::RadToDegrees(atan(delta[1] / delta[0]));
	if (end[0] <= beg[0]) angle -= 180;
	line.setRotation(angle);
	line.setOrigin(0, thicness / 2.0);

	line.setFillColor(color);
	window.draw(line);
}

void dble_collisions::draw_point(
	const vl::vec2d& pos, 
	sf::RenderWindow& window, 
	sf::Color color, 
	const float& radius)
{
	static sf::CircleShape shape;
	shape.setRadius(radius);
	shape.setOrigin(radius, radius);
	shape.setFillColor(color);
	shape.setPosition(utils::vec2_to_sfml(pos));
	window.draw(shape);
}
