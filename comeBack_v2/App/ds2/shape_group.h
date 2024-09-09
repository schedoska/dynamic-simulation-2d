#pragma once

#include "vec2.h"
#include "circle_shape.h"
#include "convex_shape.h"
#include "shape.h"

namespace ds2
{
	class shape_group : public shape
	{
	public:
		shape_group();
		void add(const circle_shape& circle);
		void add(const convex_shape& convex);
		const std::vector<circle_shape>& circles() const;
		const std::vector<convex_shape>& convexes() const;
		void clear();
		void translate(const vl::vec2d& v);
		void translate_to_centroid();

		double area() const override;
		vl::vec2d centroid() const override;
		double second_moment_area() const override;
		const rect box(const vl::vec2d& pos, const double& rot) const override;
		void scale(const vl::vec2d& scale) override;

	private:
		std::vector<circle_shape> _circles;
		std::vector<convex_shape> _convexes;
	};
}

