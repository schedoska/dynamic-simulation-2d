#pragma once

#include "collision_detection.h"

namespace ds2
{
	class collision_solver
	{
	public:
		static void solve_collision(const collision_data& cd);
	};
}

