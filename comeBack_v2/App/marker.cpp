#include "marker.h"
#include "ds2/Utils.h"

marker::marker()
{
	_marker_shape.setSize(sf::Vector2f(10, 10));
	_marker_shape.setOrigin(5, 5);
	_marker_shape.setFillColor(sf::Color::White);
	_marker_shape.setOutlineThickness(3);
	_marker_shape.setOutlineColor(sf::Color::Blue);
	
	_target_body = nullptr;
	_path_length = 200;
	_path_res = 2;
	_path_step - 0;
}

void marker::draw(sf::RenderWindow& window)
{
	_marker_shape.setPosition(utils::vec2_to_sfml(pos()));
	window.draw(_marker_shape);
}

void marker::update(const sf::Window* window)
{
	if (++_path_step > _path_res) {
		_path_step = 0;
		_path.push_front(pos());
		if (_path.size() > _path_length) _path.pop_back();
	}
}

body* marker::target_body()
{
	return _target_body;
}

void marker::set_body(body* body)
{
	_target_body = body;
	if (_target_body) _marker_shape.setFillColor(sf::Color::Blue);
	else _marker_shape.setFillColor(sf::Color::White);
}

void marker::set_global_pos(const vl::vec2d& global_pos)
{
	if (_target_body) {
		_loc_pos = _target_body->local(global_pos);
	}
	else {
		_loc_pos = global_pos;
	}
}

const vl::vec2d& marker::pos() const
{
	if (_target_body) {
		return _target_body->global(_loc_pos);
	}
	else {
		return _loc_pos;
	}
}

void marker::draw_line(
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

void marker::draw_path(sf::RenderWindow& window)
{
	vl::vec2d prev = { 0,0 };
	if (_path.size() > 0) prev = _path.front();
	for (const auto& v : _path) {
		draw_line(prev, v, window, sf::Color::White, 3);
		prev = v;
	}
}
