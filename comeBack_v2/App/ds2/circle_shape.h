#pragma once

#include "shape.h"
#include "vec2.h"
#include <vector>

namespace ds2
{
	class circle_shape : public shape
	{
	public:
		circle_shape(
			const vl::vec2d& loc_pos = vl::vec2d(),
			const double& radius = 1.f);

		const double& radius() const;
		void set_radius(const double& radius);
		const vl::vec2d& loc_pos() const;
		void set_loc_pos(const vl::vec2d& pos);

		double area() const override;
		double second_moment_area() const override;
		const rect box(const vl::vec2d& pos, const double& rot) const override;
		vl::vec2d centroid() const override;
		void scale(const vl::vec2d& scale) override;
		void rotate(const double& rad) override;

	protected:
		double _radius;
		vl::vec2d _loc_pos;
	};
}

