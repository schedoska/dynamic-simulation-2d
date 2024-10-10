#pragma once

#include <SFML/Graphics.hpp>
#include "../ds2/vec.h"
#include <functional>

class polygon_tool
{
public:
	polygon_tool();
	void update(const sf::Window* window);
	void draw(sf::RenderWindow* window);
	void start_shape(std::function<void(std::vector<vl::vec2d>)> created_callback);

private:
	// is currently drawing
	bool _active; 
	sf::Vector2f _snap_pos;
	std::vector<sf::Vector2f> _vertices;	 

	sf::RectangleShape _start_vx_shape;
	sf::CircleShape _vx_shape;

	void create_shape();
	void draw_line(
		const sf::Vector2f& beg, 
		const sf::Vector2f& end, 
		const sf::Color& color,
		sf::RenderWindow* window);
	sf::Vector2f snapped_vh_line(
		const sf::Vector2f& beg, 
		const sf::Vector2f& end);

	std::function<void (std::vector<vl::vec2d>)> _created_callback;
};

namespace polygon_tool_conf
{
	constexpr float start_vertex_size = 20;
	constexpr float vertex_size = 18;
	constexpr float line_width = 4;
	constexpr float snap_len = 50;

	const sf::Color start_vertex_color = sf::Color::Blue;
	const sf::Color vertex_color = sf::Color::Green;
	const sf::Color line_color = sf::Color::White;
	const sf::Color active_line_color = sf::Color::Red;
}
