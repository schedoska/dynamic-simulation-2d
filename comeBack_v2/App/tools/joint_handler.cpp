#include "joint_handler.h"
#include "../ds2/Utils.h"

joint_handler::joint_handler()
	: _mode(joint_handler_mode::a), _active(false)
{
	_border.setFillColor(sf::Color::Transparent);
	_border.setOutlineColor(sf::Color::Red);
	_border.setOutlineThickness(4);
	_border.setSize(sf::Vector2f(40, 40));
	_border.setOrigin(20, 20);

	_connected_obj_shape.setFillColor(sf::Color(0, 0, 0, 60));
	_connected_obj_shape.setSize(sf::Vector2f(20, 20));
	_connected_obj_shape.setOrigin(10, 10);
}

void joint_handler::update(const sf::Window* window)
{
	if (_target == nullptr) return;
	bool clicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));

	if (!clicked && _active) {
		_active = false;
	}
	else if (_active) {
		_border.setPosition(mouse_pos - _grab_pos);
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

void joint_handler::draw(sf::RenderWindow* window)
{
	if (_target == nullptr) return;

	sf::Color lc = sf::Color(0, 0, 0, 60);
	if (_target->body_a()) {
		vl::vec2d ap = _target->body_a()->pos();
		draw_line(_target->joint()->global_a(), ap, lc, window);
		_connected_obj_shape.setPosition(utils::vec2_to_sfml(ap));
		window->draw(_connected_obj_shape);
	}
	if (_target->body_b()) {
		vl::vec2d bp = _target->body_b()->pos();
		draw_line(_target->joint()->global_b(), bp, lc, window);
		_connected_obj_shape.setPosition(utils::vec2_to_sfml(bp));
		window->draw(_connected_obj_shape);
	}
	window->draw(_border);
}

void joint_handler::set_target(dble_joint* target, joint_handler_mode mode)
{
	_target = target;
	if (_target == nullptr) return;
	_mode = mode;
	set_border();
}

void joint_handler::set_border()
{
	sf::Vector2f pos;
	if (_mode == joint_handler_mode::a || _mode == joint_handler_mode::both) {
		_border.setPosition(utils::vec2_to_sfml(_target->joint()->global_a()));
	}
	else {
		_border.setPosition(utils::vec2_to_sfml(_target->joint()->global_b()));
	}
}

dble_joint* joint_handler::target() const
{
	return _target;
}

const bool joint_handler::is_active() const
{
	return _active;
}

void joint_handler::target_set_object(body* obj)
{
	if (_target == nullptr) return;
	if ((obj == _target->body_a() || obj == _target->body_b()) && obj) return;

	vl::vec2d pos_a = _target->joint()->global_a();
	vl::vec2d pos_b = _target->joint()->global_b();

	if (_mode == joint_handler_mode::a) {
		_target->set_body_a(obj);
		obj ? _target->joint()->set_loc_a(obj->local(pos_a)) : _target->joint()->set_loc_a(pos_a);
	}
	if (_mode == joint_handler_mode::b) {
		_target->set_body_b(obj);
		obj ? _target->joint()->set_loc_b(obj->local(pos_b)) : _target->joint()->set_loc_b(pos_b);
	}

	if (_mode == joint_handler_mode::both) {
		if (obj == nullptr) {
			_target->set_body_a(obj);
			_target->set_body_b(obj);
			_target->joint()->set_loc_a(pos_a);
			_target->joint()->set_loc_b(pos_b);
		}
		else {
			if (_turn) {
				_target->set_body_a(obj);
				_target->joint()->set_loc_a(obj->local(pos_a));
			}
			else {
				_target->set_body_b(obj);
				_target->joint()->set_loc_b(obj->local(pos_b));
			}
		}
	}
	_turn = !_turn;
}

void joint_handler::set_target()
{
	vl::vec2d new_pos = utils::sfml_to_vec2d(_border.getPosition());
	vl::vec2d loc_a = _target->body_a() ? _target->body_a()->local(new_pos) : new_pos;
	vl::vec2d loc_b = _target->body_b() ? _target->body_b()->local(new_pos) : new_pos;

	if (_mode == joint_handler_mode::a || _mode == joint_handler_mode::both) {
		_target->joint()->set_loc_a(loc_a);
	}
	if (_mode == joint_handler_mode::b || _mode == joint_handler_mode::both) {
		_target->joint()->set_loc_b(loc_b);
	}
}

void joint_handler::draw_line(
	const vl::vec2d& beg,
	const vl::vec2d& end,
	const sf::Color& color,
	sf::RenderWindow* window)
{
	static sf::RectangleShape line;
	line.setPosition(utils::vec2_to_sfml(beg));
	vl::vec2d delta = beg - end;
	double len = delta.len();

	line.setSize(sf::Vector2f(len, 4));
	float angle = utils::RadToDegrees(atan(delta[1] / delta[0]));
	if (end[0] <= beg[0]) angle -= 180;
	line.setRotation(angle);

	line.setFillColor(color);
	window->draw(line);
}