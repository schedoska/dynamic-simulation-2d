#pragma once

#include "vec2.h"

namespace ds2
{
	struct axis_projection_data
	{
		axis_projection_data();
		bool is_overlaping;
		float penetration;
		vl::vec2d closest_point;	/* Secondary object */
		vl::vec2d collision_normal;
	};
}

