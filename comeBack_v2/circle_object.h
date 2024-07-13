#pragma once

#include "object.h"

namespace ds2 
{
	class circle_object : public object {
	public:
		circle_object(const vl::vec2d& pos	= vl::vec2d(),
					  const double&	radius	= 0.0);
		double& radius();
		const double& radius() const;

	private:
		double _radius;
	};
}