#include "force_tool.h"
#include "../ds2/Utils.h"

force_tool::force_tool()
{
	_force_line.setFillColor(sf::Color::White);
}

void force_tool::update(const sf::Window* window, const sf::Time& dt)
{
	if (_target == nullptr) return;
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		_target = nullptr;
		return;
	}

	_glob_end_pos = utils::sfml_to_vec2d(sf::Mouse::getPosition(*window));

	vl::vec2d force_v = _glob_end_pos - _target->global(_loc_grab_pos);
	_target->apply_force(force_v * 10, _loc_grab_pos, dt.asSeconds());
}

void force_tool::draw(sf::RenderWindow* window)
{
	if (_target == nullptr) return;

	sf::Vector2f beg = utils::vec2_to_sfml(_target->global(_loc_grab_pos));
	sf::Vector2f end = utils::vec2_to_sfml(_glob_end_pos);
	_force_line.setPosition(beg);

	_force_line.setPosition(beg);
	sf::Vector2f delta = beg - end;
	double len = utils::sfml_to_vec2d(delta).len();

	_force_line.setSize(sf::Vector2f(len, 7));
	float angle = utils::RadToDegrees(atan(delta.y / delta.x));
	if (end.x <= beg.x) angle -= 180;
	_force_line.setRotation(angle);

	window->draw(_force_line);
}

void force_tool::set_target(body* target, const sf::Vector2i& mouse_pos)
{
	if (target == nullptr) return;
	_target = target;
	_loc_grab_pos = _target->local(utils::sfml_to_vec2d(mouse_pos));
	_glob_end_pos = utils::sfml_to_vec2d(mouse_pos);
}

body* force_tool::target() const
{
	return _target;
}
