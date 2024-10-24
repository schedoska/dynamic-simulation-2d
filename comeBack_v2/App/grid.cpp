#include "grid.h"

grid::grid()
{
	_grid_size = 30;	
	_active = true;
	create_dot_grid();
}

void grid::update(const sf::Window* window)
{
}

void grid::draw(sf::RenderWindow* window)
{
	if (!_active) return;
	window->draw(_grid_sprite);
}

vl::vec2d grid::snap(const vl::vec2d& pos) const
{
	if (!_active) return pos;
	int x = (int(pos[0] + _grid_size * 0.5f) / int(_grid_size)) * _grid_size;
	int y = (int(pos[1] + _grid_size * 0.5f) / int(_grid_size)) * _grid_size;
	return vl::vec2d(x, y);
}

sf::Vector2f grid::snap(const sf::Vector2f& pos) const
{
	if (!_active) return pos;
	int x = (int(pos.x + _grid_size * 0.5f) / int(_grid_size)) * _grid_size;
	int y = (int(pos.y + _grid_size * 0.5f) / int(_grid_size)) * _grid_size;
	return sf::Vector2f(x, y);
}

const float& grid::grid_size() const
{
	return _grid_size;
}

void grid::set_grid_size(const float& size)
{
	_grid_size = size;
	create_dot_grid();
}

const bool& grid::active() const
{
	return _active;
}

void grid::set_active(bool active)
{
	_active = active;
}

void grid::draw_line(
	const sf::Vector2f& beg, 
	const sf::Vector2f& end, 
	sf::RenderTarget& target,
	sf::Color color, 
	const float& thicness)
{
	static sf::RectangleShape line;
	line.setPosition(beg);
	vl::vec2d delta = utils::sfml_to_vec2d(beg - end);

	line.setSize(sf::Vector2f(delta.len(), thicness));
	float angle = utils::RadToDegrees(atan(delta[1] / delta[0]));
	if (end.x <= beg.x) angle -= 180;
	line.setRotation(angle);
	line.setOrigin(0, thicness / 2.0);

	line.setFillColor(color);
	target.draw(line);
}

void grid::draw_point(
	const sf::Vector2f& pos, 
	sf::RenderTarget& target,
	sf::Color color, 
	const float& radius)
{
	static sf::CircleShape shape;
	shape.setRadius(radius);
	shape.setOrigin(radius, radius);
	shape.setFillColor(color);
	shape.setPosition(pos);
	target.draw(shape);
}

void grid::create_line_grid()
{
	sf::RenderTexture render_txt;
	render_txt.create(_app_width, _app_height);

	int y = _grid_size;
	while (y < _app_height) {
		const int br = 160;
		draw_line(sf::Vector2f(0, y), sf::Vector2f(_app_width, y), render_txt, sf::Color(br, br, br), 1);
		y += _grid_size;
	}
	int x = _grid_size;
	while (x < _app_width) {
		const int br = 160;
		draw_line(sf::Vector2f(x, 0), sf::Vector2f(x, _app_height), render_txt, sf::Color(br, br, br), 1);
		x += _grid_size;
	}

	_grid_txt = render_txt.getTexture();
	_grid_sprite.setTexture(_grid_txt);
}

void grid::create_dot_grid()
{
	sf::RenderTexture render_txt;
	render_txt.create(_app_width, _app_height);

	int y = _app_height;
	while (y > 0) {
		int x = _grid_size;
		while (x < _app_width) {
			int br = 52;
			draw_point(sf::Vector2f(x, y), render_txt, sf::Color(br, br, br), 2);
			x += _grid_size;
		}
		y -= _grid_size;
	}

	_grid_txt = render_txt.getTexture();
	_grid_sprite.setTexture(_grid_txt);
}
