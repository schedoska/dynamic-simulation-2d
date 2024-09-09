#include "DebugObject.h"
#include "ds2/Utils.h"

DebugObject::DebugObject(vl::vec2d pos)
	: object(pos)
{
}

void DebugObject::draw(sf::RenderWindow& window)
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
	/*
	ds2::rect b = shape().box(pos(), rot());
	utils::drawLine(b.pos, b.pos + vl::vec2d(b.w, 0), window, sf::Color::Red);
	utils::drawLine(b.pos + vl::vec2d(b.w, 0), b.pos + vl::vec2d(b.w, b.h), window, sf::Color::Red);
	utils::drawLine(b.pos + vl::vec2d(b.w, b.h), b.pos + vl::vec2d(0, b.h), window, sf::Color::Red);
	utils::drawLine(b.pos + vl::vec2d(0, b.h), b.pos, window, sf::Color::Red);
	*/
}

void DebugObject::update_shape()
{
	_circle_gr.clear();
	_convex_gr.clear();

	for (const auto& i : shape().convexes()) {
		const std::vector<vl::vec2d>& vert = i.vertices();
		sf::ConvexShape cs;
		cs.setFillColor(sf::Color::Transparent);
		cs.setOutlineThickness(2);
		cs.setOutlineColor(sf::Color::White);
		cs.setPointCount(vert.size());
		for (int i = 0; i < vert.size(); i++) {
			cs.setPoint(i, utils::vec2_to_sfml(vert[i]));
		}
		_convex_gr.push_back(cs);
	}
	for (const auto& i : shape().circles()) {
		sf::CircleShape cs;
		cs.setFillColor(sf::Color::Transparent);
		cs.setOutlineThickness(2);
		cs.setOutlineColor(sf::Color::White);
		cs.setRadius(i.radius());

		vl::vec2d offset = i.loc_pos() * -1.f + vl::vec2d(i.radius(), i.radius());
		cs.setOrigin(utils::vec2_to_sfml(offset));
		_circle_gr.push_back(cs);
	}
}
