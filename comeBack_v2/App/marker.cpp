#include "marker.h"
#include "ds2/Utils.h"

marker::marker()
{
	_path_color = sf::Color::White;

	_marker_shape.setSize(sf::Vector2f(10, 10));
	_marker_shape.setOrigin(5, 5);
	_marker_shape.setFillColor(sf::Color::White);
	_marker_shape.setOutlineThickness(3);
	_marker_shape.setOutlineColor(sf::Color::Blue);
	
	_target_body = nullptr;

	_path_res = 0.025;
	_path_max_len = 1.0;
	_path_current_len = 0.0;
	_path_res_acc = 0.0;
}

marker::marker(const double& path_max_len, const double& path_res) : marker()
{
	_path_max_len = path_max_len;
	_path_res = path_res;
}

void marker::draw(sf::RenderWindow& window)
{
	_marker_shape.setPosition(utils::vec2_to_sfml(pos()));
	window.draw(_marker_shape);
}

void marker::update(const sf::Window* window, const double& dt)
{
	_path_res_acc += dt;

	if (_path_res_acc > _path_res) {
		double st = _path_res_acc;
		_path_res_acc = 0.0;
		_path.push_front(pos());
		_path_current_len += st;

		while (_path_current_len > _path_max_len) {
			_path.pop_back();
			_path_current_len -= st;
		}
	}
}

const body* marker::target_body() const
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

const vl::vec2d& marker::loc_pos() const
{
	return _loc_pos;
}

void marker::set_loc_pos(const vl::vec2d& loc_pos)
{
	_loc_pos = loc_pos;
}

const double marker::path_res() const
{
	return _path_res;
}

void marker::set_path_res(const double& path_res)
{
	_path_res = path_res;
}

const double marker::path_max_len() const
{
	return _path_max_len;
}

void marker::set_max_path_len(const double& max_path_len)
{
	_path_max_len = max_path_len;
}

const sf::Color& marker::path_color() const
{
	return _path_color;
}

void marker::set_path_color(const sf::Color& color)
{
	_path_color = color;
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

	double step = 255.0 / _path.size();
	float br = 255;

	for (const auto& v : _path) {
		sf::Color c = _path_color;
		c.a = (br -= step);
		draw_line(prev, v, window, c, 3);
		prev = v;
	}
}
