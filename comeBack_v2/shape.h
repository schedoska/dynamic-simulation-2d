#pragma once

#include "vec2.h"

namespace ds2
{
	struct rect
	{
		rect();
		rect(const vl::vec2d& pos, const double& w, const double& h);
		vl::vec2d pos;		// Top left corner
		double w, h;		// width and height
		void expand(const vl::vec2d& v);
		void expand(const rect& rect);
	};

	class shape
	{
	public:
		virtual ~shape() {};
		virtual double area() const = 0;
		virtual vl::vec2d centroid() const = 0;

		/* Second moment of area - used to calculate inertia */
		virtual double second_moment_area() const = 0;	
		virtual const rect box(const vl::vec2d& pos, const double& rot) const;

	protected:
	};
}

