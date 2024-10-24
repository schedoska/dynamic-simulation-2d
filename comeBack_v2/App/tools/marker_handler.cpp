#include "marker_handler.h"

marker_handler::marker_handler()
{
	_target = nullptr;
	_border.setFillColor(sf::Color::Transparent);
	_border.setOutlineColor(sf::Color::Red);
	_border.setOutlineThickness(4);

	constexpr double size = 40.0;
	_border.setSize(sf::Vector2f(size, size));
	_border.setOrigin(size / 2.0, size / 2.0);

	_connected_obj_shape.setFillColor(sf::Color(0, 0, 0, 60));
	_connected_obj_shape.setSize(sf::Vector2f(20, 20));
	_connected_obj_shape.setOrigin(10, 10);

	_grid = nullptr;
}

void marker_handler::update(const sf::Window* window)
{
	if (_target == nullptr) return;
	bool clicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));

	if (!clicked && _active) {
		_active = false;
	}
	else if (_active) {
		sf::Vector2f new_pos = mouse_pos - _grab_pos;
		if (_grid) new_pos = _grid->snap(new_pos);
		_border.setPosition(new_pos);
		set_target();
		return;
	}

	_border.setOutlineColor(sf::Color::Red);
	bool contain = _border.getGlobalBounds().contains(mouse_pos);
	if (contain && !_active && clicked) {
		_active = true;
		_border.setOutlineColor(sf::Color::Black);
		_grab_pos = mouse_pos - _border.getPosition();
	}
}

void marker_handler::draw(sf::RenderWindow* window)
{
	if (!_target) return;

	window->draw(_border);
}

void marker_handler::set_target(marker* target)
{
	_target = target;
	if (_target) set_border();
}

void marker_handler::set_border()
{
	if (!_target) return;
	_border.setPosition(utils::vec2_to_sfml(_target->pos()));
}

void marker_handler::set_grid(grid* g)
{
	_grid = g;
}

const bool marker_handler::is_active() const
{
	return _active;
}

void marker_handler::target_set_object(body* obj)
{
	if (_target == nullptr) return;

	vl::vec2d m_pos = _target->pos();
	_target->set_body(obj);
	_target->set_global_pos(m_pos);
}

marker* marker_handler::target() const
{
	return _target;
}

void marker_handler::set_target()
{
	_target->set_global_pos(utils::sfml_to_vec2d(_border.getPosition()));
}
