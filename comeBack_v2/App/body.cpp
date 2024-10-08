#include "Body.h"
#include "ds2/Utils.h"

body::body(const std::string& name)
	: _name(name)
{
	set_color(sf::Color(209, 69, 69));
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

void body::set_color(const sf::Color& color)
{
	_color = color;
}

const sf::Color& body::color() const
{
	return _color;
}

void body::set_name(const std::string& name)
{
	_name = name;
}

const std::string& body::name() const
{
	return _name;
}

void body::update_shape()
{
	_circle_gr.clear();
	_convex_gr.clear();

	for (const auto& i : shape().convexes()) {
		const std::vector<vl::vec2d>& vert = i.vertices();
		sf::ConvexShape cs;
		cs.setFillColor(_color);
		cs.setPointCount(vert.size());
		for (int i = 0; i < vert.size(); i++) {
			cs.setPoint(i, utils::vec2_to_sfml(vert[i]));
		}
		_convex_gr.push_back(cs);
	}
	for (const auto& i : shape().circles()) {
		sf::CircleShape cs;
		cs.setFillColor(_color);
		cs.setRadius(i.radius());
		vl::vec2d offset = i.loc_pos() * -1.f + vl::vec2d(i.radius(), i.radius());
		cs.setOrigin(utils::vec2_to_sfml(offset));
		_circle_gr.push_back(cs);
	}
}
