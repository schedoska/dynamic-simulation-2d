#include "force_tool.h"
#include "../ds2/Utils.h"

force_tool::force_tool()
{
	_force_line.setFillColor(sf::Color::White);
	_proportional_froce = false;
	_strength = 40.f;
	_damping = 3.f;
	_rot_damping = 1.f;
}

void force_tool::update(const sf::Window* window, const sf::Time& dt)
{
	if (_target == nullptr) return;
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		_target = nullptr;
		return;
	}

	_glob_end_pos = utils::sfml_to_vec2d(sf::Mouse::getPosition(*window));

	vl::vec2d dv = _glob_end_pos - _target->global(_loc_grab_pos);
	double len = dv.len();
	dv.normalize();
	double rel_vel = (_target->vel()).dot(dv);
	double f = _strength * len - _damping * rel_vel;
	_proportional_froce ? f *= _target->mass() : f;

	_target->apply_force(dv * _strength * len - _target->vel() * _damping, _loc_grab_pos, dt.asSeconds());
	_target->rot_vel() -= _rot_damping * _target->rot_vel() * dt.asSeconds();
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

	constexpr double min_white_len = 50;
	constexpr double max_black_len = 200;

	float b = 270 - 0.2 * len;
	b = std::max(std::min(b, 255.f), 180.f);
	_force_line.setFillColor(sf::Color(b, b, b, 200));
	float thicness = 10.0 - 0.01 * len;
	thicness = std::max(std::min(thicness, 7.f), 1.f);

	_force_line.setSize(sf::Vector2f(len, thicness));
	_force_line.setOrigin(0, thicness / 2.0);
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

void force_tool::set_strength(const float& strength)
{
	_strength = strength;
}

void force_tool::set_damping(const float& damping)
{
	_damping = damping;
}

void force_tool::set_rot_damping(const float& rot_damping)
{
	_rot_damping = rot_damping;
}

void force_tool::set_proportional_force(const bool proportional_force)
{
	_proportional_froce = proportional_force;
}

const float& force_tool::strength() const
{
	return _strength;
}

const float& force_tool::damping() const
{
	return _damping;
}

const float& force_tool::rot_damping() const
{
	return _rot_damping;
}

bool force_tool::proportional_force() const
{
	return _proportional_froce;
}
