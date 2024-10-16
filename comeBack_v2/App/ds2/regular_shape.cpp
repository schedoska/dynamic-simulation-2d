#include "regular_shape.h"

#define _USE_MATH_DEFINES
#include <math.h>

ds2::regular_shape::regular_shape(const unsigned int& sides, const double& radius)
{
	assert((sides > 2) && "Regular shape: 'siedes' arg must be bigger then 2");

	const double ds = 2.0 * M_PI / (double)sides;
	vl::vec2d v = { 0,-1 };
	if (sides % 2 == 0) {
		v = vl::rotate(v, ds / 2.0);
	}
	for (size_t i = 0; i < sides; ++i) {
		_vertices.push_back(v * radius);
		v = vl::rotate(v, ds);
	}
}

unsigned int ds2::regular_shape::sides() const
{
	return _vertices.size();
}

double ds2::regular_shape::radius() const
{
	return _vertices[0].len();
}
