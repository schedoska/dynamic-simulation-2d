#pragma once

#include "body.h"
#include <SFML/Graphics.hpp>
#include <deque>

class marker
{
public:
	marker();
	marker(const double& path_max_len, const double& path_res);

	void draw(sf::RenderWindow& window);
	void draw_path(sf::RenderWindow& window);
	void update(const sf::Window* window, const double& dt);

	const body* target_body() const;
	void set_body(body* body);
	void set_global_pos(const vl::vec2d& global_pos);
	const vl::vec2d& pos() const;

	const vl::vec2d& loc_pos() const;
	void set_loc_pos(const vl::vec2d& loc_pos);
	const double path_res() const;
	void set_path_res(const double& path_res);
	const double path_max_len() const;
	void set_max_path_len(const double& max_path_len);

	const sf::Color& path_color() const;
	void set_path_color(const sf::Color& color);

private:
	std::deque<vl::vec2d> _path;

	double _path_res;			// in seconds: time between each vecrtex
	double _path_res_acc;		// in seconds;

	double _path_max_len;	    // in seconds
	double _path_current_len;	// in seconds

	body* _target_body;
	vl::vec2d _loc_pos;

	sf::RectangleShape _marker_shape;
	sf::Color _path_color;

	void draw_line(
		const vl::vec2d& beg,
		const vl::vec2d& end,
		sf::RenderWindow& window,
		sf::Color color,
		const float& thicness);
};

