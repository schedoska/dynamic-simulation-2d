#pragma once

#include "shape.h"
#include "vec2.h"
#include <vector>

namespace ds2
{
	class convex_shape : public shape
	{
	public:
		convex_shape();
		virtual ~convex_shape() {};

		void add(const vl::vec2d& vertex);
		void clear();
		const std::vector<vl::vec2d>& vertices() const;
		void translate(const vl::vec2d& v);

		double area() const override;
		vl::vec2d centroid() const override;
		double second_moment_area() const override;
		const rect box(const vl::vec2d& pos, const double& rot) const override;
		void scale(const vl::vec2d& scale) override;
		void rotate(const double& rad) override;

	protected:
		std::vector<vl::vec2d> _vertices;
	};
}
