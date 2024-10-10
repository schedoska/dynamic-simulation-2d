#include "body_handler.h"
#include "../ds2/Utils.h"

body_handler::body_handler()
{
	_active = false;
	_target = nullptr;

	for (auto& i : _stretchers) {
		i.setFillColor(handler_conf::handler_color);
		i.setRadius(handler_conf::handler_radius);
		i.setOrigin(handler_conf::handler_radius, handler_conf::handler_radius);
	}
	_border.setFillColor(sf::Color::Transparent);
	_border.setOutlineColor(handler_conf::border_color);
	_border.setOutlineThickness(handler_conf::border_width);

	_rotator.setFillColor(handler_conf::handler_color);
	_rotator.setRadius(handler_conf::handler_radius);
	_rotator.setOrigin(handler_conf::handler_radius, handler_conf::handler_radius);
}

void body_handler::update(const sf::Window* window)
{
	if (_target == nullptr) return;
	bool clicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));

	if (!clicked && _active) {
		_active = false;
	}
	else if (_active) {
		update_active(mouse_pos);
		update_handlers_pos();
		set_target();
		return;
	}

	for (auto& i : _stretchers) i.setFillColor(handler_conf::handler_color);
	_rotator.setFillColor(handler_conf::handler_color);
	_border.setOutlineColor(handler_conf::border_color);

	for (int i = handler::top; i != handler::rotator; ++i) {
		handler h = static_cast<handler>(i);
		bool contains = _stretchers[i].getGlobalBounds().contains(mouse_pos);
		if (contains && !_active) {
			_stretchers[i].setFillColor(handler_conf::handler_hover_color);
			_current_handler = h;
			_active = true;
		}
	}
	bool contains_rotator = _rotator.getGlobalBounds().contains(mouse_pos);
	if (contains_rotator && !_active) {
		_rotator.setFillColor(handler_conf::handler_hover_color);
		_current_handler = handler::rotator;
		_active = true;
	}
	if (borders_contains(mouse_pos) && !_active) {
		_current_handler = handler::position;
		_active = true;
		_grab_pos = mouse_pos - _border.getPosition();
	}
}

void body_handler::draw(sf::RenderWindow* window)
{
	if (_target == nullptr) return;
	window->draw(_border);
	window->draw(_rotator);
	for (const auto& i : _stretchers) window->draw(i);
}

void body_handler::set_target(body* target)
{
	_target = target;
	if (_target == nullptr) {
		_active = false;
		return;
	}

	// Decide wheter to use equal axis scling mode
	eql_axis_mode = _target->shape().circles().size() > 0 ? true : false;
	set_border();
	update_handlers_pos();
	return;
}

body* body_handler::target() const
{
	return _target;
}

const bool body_handler::is_active() const
{
	return _active;
}

void body_handler::update_handlers_pos()
{
	double w2 = _border.getSize().x / 2.0;
	double h2 = _border.getSize().y / 2.0;
	constexpr double r = handler_conf::handler_radius;

	_stretchers[handler::top].setOrigin(r, h2 + r);
	_stretchers[handler::right].setOrigin(-w2 + r, r);
	_stretchers[handler::bottom].setOrigin(r, -h2 + r);
	_stretchers[handler::left].setOrigin(w2 + r, r);

	_stretchers[handler::top_left].setOrigin(w2 + r, h2 + r);
	_stretchers[handler::top_right].setOrigin(-w2 + r, h2 + r);
	_stretchers[handler::bottom_right].setOrigin(-w2 + r, -h2 + r);
	_stretchers[handler::bottom_left].setOrigin(w2 + r, -h2 + r);

	for (auto& i : _stretchers) {
		i.setPosition(_border.getPosition());
		i.setRotation(_border.getRotation());
	}

	sf::Vector2f size = _border.getSize();
	_border.setOrigin(size.x / 2.0, size.y / 2.0);

	_rotator.setOrigin(r, h2 + handler_conf::rotator_top_dist + r);
	_rotator.setPosition(_border.getPosition());
	_rotator.setRotation(_border.getRotation());
}

void body_handler::update_active(const sf::Vector2f& mouse_pos)
{
	if (_current_handler == handler::position) {
		_border.setOutlineColor(handler_conf::border_hover_color);
		_border.setPosition(mouse_pos - _grab_pos);
		return;
	}
	else if (_current_handler == handler::rotator) {
		vl::vec2d dv = utils::sfml_to_vec2d(mouse_pos) - utils::sfml_to_vec2d(_border.getPosition());
		vl::vec2d ref = { 0, -1 };
		double ang = utils::angle(ref, dv);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			ang = snap_angle(ang, 8);
		_border.setRotation(utils::RadToDegrees(ang));
	}

	sf::Vector2f loc_mouse_pos = mouse_pos - _border.getPosition();
	vl::vec2d lmpv = utils::sfml_to_vec2d(loc_mouse_pos);
	vl::vec2d hnv = vl::rotate(vl::vec2d(0, 1), utils::DegreesToRad(_border.getRotation()));
	vl::vec2d wnv = vl::rotate(vl::vec2d(1, 0), utils::DegreesToRad(_border.getRotation()));
	double h_val = hnv.dot(lmpv);
	double w_val = wnv.dot(lmpv);
	
	double h_val_xx = h_val;
	double w_val_xx = w_val;
	if (eql_axis_mode) {
		double avg_val = (std::abs(h_val) + std::abs(w_val)) / 2.0;
		h_val_xx = avg_val * (double)((h_val > 0) - (h_val < 0));
		w_val_xx = avg_val * (double)((w_val > 0) - (w_val < 0));
	}

	auto stretch = [=](handler h, const sf::Vector2f& v, bool b = true) {
		if (!b) return;
		_stretchers[h].setOrigin(v);
		stretch_border(h);
	};

	constexpr double min_b = handler_conf::min_border_size;	// minimal border size dimension
	constexpr double r = handler_conf::handler_radius;  // radius of stretrcher handler
	switch (_current_handler)
	{
	case handler::top:
		stretch(handler::top, sf::Vector2f(r, -std::min(h_val, -min_b) + r));
		stretch(handler::right, sf::Vector2f(-std::max(-h_val, min_b) + r, r), eql_axis_mode);
		break;
	case handler::bottom:
		stretch(handler::bottom, sf::Vector2f(r, -std::max(h_val, min_b) + r));
		stretch(handler::right, sf::Vector2f(-std::max(h_val, min_b) + r, r), eql_axis_mode);
		break;
	case handler::right:
		stretch(handler::right, sf::Vector2f(-std::max(w_val, min_b) + r, r));
		stretch(handler::top, sf::Vector2f(r, -std::min(-w_val, -min_b) + r), eql_axis_mode);
		break;
	case handler::left:
		stretch(handler::left, sf::Vector2f(-std::min(w_val, -min_b) + r, r));
		stretch(handler::top, sf::Vector2f(r, -std::min(w_val, -min_b) + r), eql_axis_mode);
		break;
	case handler::top_left:
		stretch(handler::top, sf::Vector2f(r, -std::min(h_val_xx, -min_b) + r));
		stretch(handler::left, sf::Vector2f(-std::min(w_val_xx, -min_b) + r, r));
		break;
	case handler::top_right:
		stretch(handler::top, sf::Vector2f(r, -std::min(h_val_xx, -min_b) + r));
		stretch(handler::right, sf::Vector2f(-std::max(w_val_xx, min_b) + r, r));
		break;
	case handler::bottom_right:
		stretch(handler::bottom, sf::Vector2f(r, -std::max(h_val_xx, min_b) + r));
		stretch(handler::right, sf::Vector2f(-std::max(w_val_xx, min_b) + r, r));
		break;
	case handler::bottom_left:
		stretch(handler::bottom, sf::Vector2f(r, -std::max(h_val_xx, min_b) + r));
		stretch(handler::left, sf::Vector2f(-std::min(w_val_xx, -min_b) + r, r));
		break;
	}
}

void body_handler::stretch_border(handler h)
{
	sf::Vector2f hv = _stretchers[handler::top].getOrigin() - _stretchers[handler::bottom].getOrigin();
	sf::Vector2f wv = _stretchers[handler::right].getOrigin() - _stretchers[handler::left].getOrigin();
	sf::Vector2f size_new = sf::Vector2f(utils::sfml_to_vec2d(wv).len(), utils::sfml_to_vec2d(hv).len());

	sf::Vector2f size_diff = size_new - _border.getSize();
	_border.setSize(size_new);

	sf::Vector2f sdr = utils::rotate(size_diff, 0.0174532925 * _border.getRotation());
	if (h == handler::top || h == handler::left) {
		_border.setPosition(_border.getPosition() - (float)0.5 * sdr);
	}
	else {
		_border.setPosition(_border.getPosition() + (float)0.5 * sdr);
	}
}

bool body_handler::borders_contains(const sf::Vector2f& v)
{
	vl::vec2d origin_offset = {
		handler_conf::handler_radius,
		handler_conf::handler_radius
	};
	auto get_vec2d = [=](handler h) {
		sf::Vector2f v = _stretchers[h].getGlobalBounds().getPosition();
		return utils::sfml_to_vec2d(v);
	};
	vl::vec2d vs2d[4] = {
		get_vec2d(handler::top_left) + origin_offset,
		get_vec2d(handler::top_right) + origin_offset,
		get_vec2d(handler::bottom_right) + origin_offset,
		get_vec2d(handler::bottom_left) + origin_offset
	};
	vl::vec2d v2d = utils::sfml_to_vec2d(v);

	for (size_t i = 0; i < 4; ++i) {
		vl::vec2d dp = vs2d[(i + 1) % 4] - vs2d[i];
		vl::vec2d dv = v2d - vs2d[i];
		double s = vl::cross(dp, dv);
		if (s < 0) return false;
	}
	return true;
}

double body_handler::snap_angle(const double& rad, const int n)
{
	const double m_pi = acos(-1);
	double step = (2.0 * m_pi / n);
	double norm = (2.0 * rad + step) / (4.0 * m_pi);
	return (int)(norm * n) * step;
}

void body_handler::set_target()
{
	ds2::rect tb = _target->shape().box(_target->pos(), 0);
	sf::Vector2f b_size = _border.getSize()
		- sf::Vector2f(handler_conf::border_margin, handler_conf::border_margin);
	vl::vec2d s = { b_size.x / tb.w, b_size.y / tb.h };
	_target->shape().scale(s);
	_target->rot() = utils::DegreesToRad(_border.getRotation());

	vl::vec2d gc = tb.pos + vl::vec2d(tb.w, tb.h) * 0.5;
	vl::vec2d gcmc = gc - _target->pos();
	gcmc = vl::rotate(gcmc, _target->rot());
	_target->pos() = utils::sfml_to_vec2d(_border.getPosition()) - gcmc;
	_target->update_shape();
	_target->adjust_inertia();
}

void body_handler::set_border()
{
	ds2::rect tb = _target->shape().box(_target->pos(), 0);
	vl::vec2d gc = tb.pos + vl::vec2d(tb.w, tb.h) * 0.5;
	vl::vec2d gcmc = gc - _target->pos();
	gcmc = vl::rotate(gcmc, _target->rot());

	constexpr float m = handler_conf::border_margin;
	_border.setSize(sf::Vector2f(tb.w + m, tb.h + m));
	_border.setOrigin(0.5f * _border.getSize());
	_border.setRotation(utils::RadToDegrees(_target->rot()));
	_border.setPosition(utils::vec2_to_sfml(_target->pos() + gcmc));
	update_handlers_pos();
}


