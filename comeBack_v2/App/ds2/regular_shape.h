#pragma once

#include "concave_shape.h"

namespace ds2
{
	class regular_shape : public convex_shape
	{
	public:
		regular_shape(const unsigned int& sides, const double& radius);
		unsigned int sides() const;
		double radius() const;
	};
}

