#include "Body.h"
#include "ds2/Utils.h"

body::body(const unsigned int& id)
{
	_id = id;
	_name = ""; 
	set_fill_color(sf::Color(rand() % 255, rand() % 255, rand() % 255));
}

body::body(const unsigned int& id, const std::string& name, ds2::shape_group shape, const vl::vec2d& pos)
{
	_id = id;
	_name = name;
	set_fill_color(sf::Color(rand() % 255, rand() % 255, rand() % 255));
	_shape = shape;
	_pos = pos;
	update_shape();
}

void body::draw(sf::RenderWindow& window)
{
	for (auto& i : _circle_gr) {
		i.setPosition(utils::vec2_to_sfml(pos()));
		i.setRotation(utils::RadToDegrees(rot()));
		window.draw(i);
	}
	for (auto& i : _convex_gr) {
		i.setPosition(utils::vec2_to_sfml(pos()));
		i.setRotation(utils::RadToDegrees(rot()));
		window.draw(i);
	}
}

void body::set_fill_color(const sf::Color& fill_color)
{
	_fill_color = fill_color;
}

const sf::Color& body::fill_color() const
{
	return _fill_color;
}

void body::set_name(const std::string& name)
{
	_name = name;
}

const std::string& body::name() const
{
	return _name;
}

const unsigned int& body::id() const
{
	return _id;
}

void body::set_id(const unsigned int& id)
{
	_id = id;
}

void body::set_body_display_mode(display_mode mode)
{
	sf::Color c;
	switch (mode) {
	case display_mode::object_color:
		c = _fill_color;			break;
	case display_mode::off:
		c = sf::Color::Transparent; break;
	case display_mode::white:
		c = sf::Color::White;		break;
	}
	for (auto& i : _convex_gr) i.setFillColor(c);
	for (auto& i : _circle_gr) i.setFillColor(c);
}

void body::set_edges_display_mode(display_mode mode)
{
	sf::Color c;
	switch (mode) {
	case display_mode::object_color:
		c = _fill_color;			break;
	case display_mode::off:
		c = sf::Color::Transparent; break;
	case display_mode::white:
		c = sf::Color::White;		break;
	}
	for (auto& i : _convex_gr) {
		i.setOutlineColor(c);
	}
	for (auto& i : _circle_gr) {
		i.setOutlineColor(c);
	}
}

void body::update_shape()
{
	_circle_gr.clear();
	_convex_gr.clear();

	for (const auto& i : shape().convexes()) {
		const std::vector<vl::vec2d>& vert = i.vertices();
		sf::ConvexShape cs;
		cs.setFillColor(_fill_color);
		cs.setOutlineThickness(_outline_thicness);
		cs.setPointCount(vert.size());
		for (int i = 0; i < vert.size(); i++) {
			cs.setPoint(i, utils::vec2_to_sfml(vert[i]));
		}
		_convex_gr.push_back(cs);
	}
	for (const auto& i : shape().circles()) {
		sf::CircleShape cs;
		cs.setFillColor(_fill_color);
		cs.setOutlineThickness(_outline_thicness);
		cs.setRadius(i.radius());
		vl::vec2d offset = i.loc_pos() * -1.f + vl::vec2d(i.radius(), i.radius());
		cs.setOrigin(utils::vec2_to_sfml(offset));
		_circle_gr.push_back(cs);
	}
}
