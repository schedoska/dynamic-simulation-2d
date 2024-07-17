#pragma once

#include "collision_detection.h"
#include <SFML/Graphics.hpp>

namespace ds2
{
	class collision_solver
	{
	public:
		static void solve_collision(const collision_data& cd, sf::RenderWindow &win);
	};
}

