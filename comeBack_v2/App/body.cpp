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
	draw_outlines(window);
	draw_infill(window);
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

void body::draw_outlines(sf::RenderWindow& window)
{
	auto loop = [](int a, int n) { return ((a % n) + n) % n; };

	for (auto& i : _convex_gr) {
		size_t size = i.getPointCount();
		for (int q = 0; q < size; ++q) {
			vl::vec2d b = utils::sfml_to_vec2d(i.getPoint(loop(q, size)));
			vl::vec2d e = utils::sfml_to_vec2d(i.getPoint(loop(q + 1, size)));
			draw_line(global(b), global(e), window, sf::Color::White, _outline_thicness);
			draw_point(global(b), window, sf::Color::White, _outline_thicness / 2.0);
		}
	}
}

void body::draw_infill(sf::RenderWindow& window)
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

void body::draw_line(
	const vl::vec2d& beg, 
	const vl::vec2d& end, 
	sf::RenderWindow& window, 
	sf::Color color, 
	const float& thicness)
{
	static sf::RectangleShape line;
	line.setPosition(utils::vec2_to_sfml(beg));
	vl::vec2d delta = beg - end;

	line.setSize(sf::Vector2f(delta.len(), thicness));
	float angle = utils::RadToDegrees(atan(delta[1] / delta[0]));
	if (end[0] <= beg[0]) angle -= 180;
	line.setRotation(angle);
	line.setOrigin(0, thicness / 2.0);

	line.setFillColor(color);
	window.draw(line);
}

void body::draw_point(
	const vl::vec2d& pos, 
	sf::RenderWindow& window, 
	sf::Color color, 
	const float& radius)
{
	static sf::CircleShape shape;
	shape.setRadius(radius);
	shape.setOrigin(radius, radius);
	shape.setFillColor(color);
	shape.setPosition(utils::vec2_to_sfml(pos));
	window.draw(shape);
}

void body::update_shape()
{
	_circle_gr.clear();
	_convex_gr.clear();

	for (const auto& i : shape().convexes()) {
		const std::vector<vl::vec2d>& vert = i.vertices();
		sf::ConvexShape cs;
		cs.setFillColor(_fill_color);
		cs.setOutlineThickness(0);
		cs.setPointCount(vert.size());
		for (int i = 0; i < vert.size(); i++) {
			cs.setPoint(i, utils::vec2_to_sfml(vert[i]));
		}
		_convex_gr.push_back(cs);
	}
	for (const auto& i : shape().circles()) {
		sf::CircleShape cs;
		cs.setFillColor(_fill_color);
		cs.setOutlineThickness(0);
		cs.setRadius(i.radius());
		vl::vec2d offset = i.loc_pos() * -1.f + vl::vec2d(i.radius(), i.radius());
		cs.setOrigin(utils::vec2_to_sfml(offset));
		_circle_gr.push_back(cs);
	}
}
