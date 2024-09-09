#include "body_handler.h"
#include "body_handler.h"
#include "ds2/Utils.h"

body_handler::body_handler()
{
	_target = nullptr;
	_is_stretching = false;

	for (auto &i : _corners) {
		i.setFillColor(body_handler_conf::corner_color);
		i.setRadius(body_handler_conf::corner_radius);
		i.setOrigin(
			body_handler_conf::corner_radius,
			body_handler_conf::corner_radius);
	}
	for (auto &i : _borders) {
		i.setFillColor(body_handler_conf::border_color);
	}
}

void body_handler::update(const sf::Window* window)
{
	if (_target == nullptr) return;
	update_corner_border_pos();
	update_corner_border_input(window);

	//if (_is_stretching) stretch_box(window);
}

void body_handler::draw(sf::RenderWindow* window)
{
	for (const auto &i : _borders) window->draw(i);
	for (const auto &i : _corners) window->draw(i);
}

void body_handler::set_target(body* target)
{
	_target = target;
	_box = _target->shape().box(_target->pos(), _target->rot());
}

void body_handler::update_corner_border_pos()
{
	ds2::rect box = _box;

	const sf::Vector2f tl = utils::vec2_to_sfml(box.pos);
	const sf::Vector2f tr = utils::vec2_to_sfml(box.pos + vl::vec2d(box.w, 0));
	const sf::Vector2f br = utils::vec2_to_sfml(box.pos + vl::vec2d(box.w, box.h));
	const sf::Vector2f bl = utils::vec2_to_sfml(box.pos + vl::vec2d(0, box.h));
	_corners[handler_type::top_left_corner - 4].setPosition(tl);
	_corners[handler_type::top_right_corner - 4].setPosition(tr);
	_corners[handler_type::bottom_right_corner - 4].setPosition(br);
	_corners[handler_type::bottom_left_corner - 4].setPosition(bl);

	constexpr double bw = body_handler_conf::border_width; // Border width
	_borders[handler_type::top_border].setPosition(tl - sf::Vector2f(0, bw / 2.0));
	_borders[handler_type::top_border].setSize(sf::Vector2f(box.w, bw));
	_borders[handler_type::bottom_border].setPosition(bl - sf::Vector2f(0, bw / 2.0));
	_borders[handler_type::bottom_border].setSize(sf::Vector2f(box.w, bw));

	_borders[handler_type::left_border].setPosition(tl - sf::Vector2f(bw / 2.0, 0));
	_borders[handler_type::left_border].setSize(sf::Vector2f(bw, box.h));
	_borders[handler_type::right_border].setPosition(tr - sf::Vector2f(bw / 2.0, 0));
	_borders[handler_type::right_border].setSize(sf::Vector2f(bw, box.h));
}

void body_handler::update_corner_border_input(const sf::Window* window)
{
	sf::Vector2f mouse_pos = (sf::Vector2f) sf::Mouse::getPosition(*window);
	bool clicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	if (!clicked) _is_stretching = false;
	if (_is_stretching) {
		stretch_box(window);
		return;
	}

	for (size_t i = 0; i < 4; ++i ) {
		sf::FloatRect fr = _corners[i].getGlobalBounds();
		if (fr.contains(mouse_pos)) {
			_corners[i].setFillColor(body_handler_conf::corner_hover_color);
			if (clicked && !_is_stretching) {
				_is_stretching = true;
				_handler = (handler_type)(i + 4);
			}
		}
		else {
			_corners[i].setFillColor(body_handler_conf::corner_color);
		}
	}
	for (size_t i = 0; i < 4; ++i) {
		sf::FloatRect fr = _borders[i].getGlobalBounds();
		if (fr.contains(mouse_pos)) {
			_borders[i].setFillColor(body_handler_conf::border_hover_color);
			if (clicked && !_is_stretching) {
				_is_stretching = true;
				_handler = (handler_type)(i);
			}
		}
		else {
			_borders[i].setFillColor(body_handler_conf::border_color);
		}
	}
}

void body_handler::stretch_box(const sf::Window* window)
{
	sf::Vector2f mouse_pos = (sf::Vector2f)sf::Mouse::getPosition(*window);
	ds2::rect temp = _box;
	double td = _box.pos[1] - mouse_pos.y;
	double ld = _box.pos[0] - mouse_pos.x;
	double rd = mouse_pos.x - (_box.pos[0] + _box.w);
	double bd = mouse_pos.y - (_box.pos[1] + _box.h);

	switch (_handler) {
	case handler_type::top_border:
		_box.pos[1] -= td;
		_box.h += td;
		break;
	case handler_type::right_border:
		_box.w += rd;
		break;
	case handler_type::bottom_border:
		_box.h += bd;
		break;
	case handler_type::left_border:
		_box.pos[0] -= ld;
		_box.w += ld;
		break;
	case handler_type::top_left_corner:
		_box.pos -= vl::vec2d(ld, td);
		_box.h += td;
		_box.w += ld;
		break;
	case handler_type::top_right_corner:
		_box.pos[1] -= td;
		_box.h += td;
		_box.w += rd;
		break;
	case handler_type::bottom_right_corner:
		_box.w += rd;
		_box.h += bd;
		break;
	case handler_type::bottom_left_corner:
		_box.pos[0] -= ld;
		_box.w += ld;
		_box.h += bd;
		break;
	}
	if (_box.h < body_handler_conf::min_body_size ||
		_box.w < body_handler_conf::min_body_size) _box = temp;
}
