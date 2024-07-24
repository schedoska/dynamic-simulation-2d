#pragma once

#include "vec2.h"
#include <vector>

namespace ds2
{
	class circle_shape
	{
	public:
		circle_shape(
			const vl::vec2d& loc_pos = vl::vec2d(),
			const double& radius = 1.f);

		const double& radius() const;
		double& radius();
		const vl::vec2d& loc_pos() const;
		vl::vec2d& loc_pos();

	protected:
		double _radius;
		vl::vec2d _loc_pos;
	};
}

