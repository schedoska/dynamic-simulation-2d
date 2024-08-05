#pragma once

#include "vec2.h"

namespace ds2
{
	class shape
	{
	public:
		virtual ~shape() {};
		virtual double area() const = 0;
		virtual vl::vec2d centroid() const = 0;

		/* Second moment of area - used to calculate inertia */
		virtual double second_moment_area() const = 0;	
	};
}

