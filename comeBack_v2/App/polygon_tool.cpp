#include "polygon_tool.h"
#include "ds2/Utils.h"

#define _USE_MATH_DEFINES
#include <math.h>

polygon_tool::polygon_tool()
	: _active(false)
{
	constexpr float start_size = polygon_tool_conf::vertex_size;
	_start_vx_shape.setFillColor(polygon_tool_conf::start_vertex_color);
	_start_vx_shape.setSize(sf::Vector2f(start_size, start_size));
	_start_vx_shape.setOrigin(start_size / 2.0, start_size / 2.0);

	constexpr float size = polygon_tool_conf::vertex_size;
	_vx_shape.setFillColor(polygon_tool_conf::vertex_color);
	_vx_shape.setRadius(size / 2.0);
	_vx_shape.setOrigin(size / 2.0, size / 2.0);
}

void polygon_tool::update(const sf::Window* window)
{
	if (!_active) return;

	static bool prev_clkd = false;
	bool clkd = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	bool snapped_start = false;
	_snap_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));

	if (_vertices.size() > 0) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
			_snap_pos = snapped_vh_line(_vertices.back(), _snap_pos);
		}
	}
	if (_vertices.size() > 2) {
		double snap_dist = utils::sfml_to_vec2d(_snap_pos - _vertices[0]).len();
		if (snap_dist < polygon_tool_conf::snap_len) {
			_snap_pos = _vertices[0];
			snapped_start = true;
		}
	}
	if (clkd && !prev_clkd) {
		if (snapped_start)
			create_shape();
		else
			_vertices.push_back(_snap_pos);
	}
	prev_clkd = clkd;
}

void polygon_tool::draw(sf::RenderWindow* window)
{
	if (!_active) return;

	const size_t size = _vertices.size();

	if (size == 0) {
		_start_vx_shape.setPosition(_snap_pos);
		window->draw(_start_vx_shape);
		return;
	}

	// draw lines
	for (size_t i = 1; i < _vertices.size(); ++i) {
		draw_line(_vertices[i - 1], _vertices[i], polygon_tool_conf::line_color, window);
	}
	// draw currently active line
	draw_line(_vertices[size - 1], _snap_pos, polygon_tool_conf::active_line_color, window);
	_vx_shape.setPosition(_snap_pos);
	window->draw(_vx_shape);
	// draw vertices
	_start_vx_shape.setPosition(_vertices[0]);
	window->draw(_start_vx_shape);
	for (size_t i = 1; i < _vertices.size(); ++i) {
		_vx_shape.setPosition(_vertices[i]);
		window->draw(_vx_shape);
	}
}

void polygon_tool::start_shape(std::function<void(std::vector<vl::vec2d>)> created_callback)
{
	_active = true;
	_vertices.clear();
	_created_callback = created_callback;
}

void polygon_tool::create_shape()
{
	if (_vertices.size() < 3) return;
	_active = false;

	std::vector<vl::vec2d> vl_vertices;
	for (const sf::Vector2f& i : _vertices) 
		vl_vertices.push_back({ i.x, i.y });
	_created_callback(vl_vertices);
}

void polygon_tool::draw_line(
	const sf::Vector2f& beg, 
	const sf::Vector2f& end, 
	const sf::Color& color,
	sf::RenderWindow* window)
{
	static sf::RectangleShape line;
	line.setPosition(beg);
	sf::Vector2f delta = beg - end;
	double len = utils::sfml_to_vec2d(delta).len();

	line.setSize(sf::Vector2f(len, polygon_tool_conf::line_width));
	float angle = utils::RadToDegrees(atan(delta.y / delta.x));
	if (end.x <= beg.x) angle -= 180;
	line.setRotation(angle);

	line.setFillColor(color);
	window->draw(line);
}

sf::Vector2f polygon_tool::snapped_vh_line(const sf::Vector2f& beg, const sf::Vector2f& end)
{
	vl::vec2d u = { 0, -1 };
	vl::vec2d dv = utils::sfml_to_vec2d(end - beg);
	vl::vec2d fin;
	double max_dot = -std::numeric_limits<double>::max();

	for (size_t i = 0; i < 4; ++i) {
		double d = u.dot(dv);
		if (d > max_dot) {
			max_dot = d;
			fin = u * d;
		}
		u = vl::rotate(u, M_PI_2);
	}
	return utils::vec2_to_sfml(fin) + beg;
}
