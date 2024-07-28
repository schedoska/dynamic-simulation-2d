#pragma once

#include "shape_group.h"

namespace ds2
{
	class concave_shape
	{
	public:
		void add(const vl::vec2d& v);
		void clear();
		shape_group generate_group();
		const std::vector<vl::vec2d>& vertices() const;

	//private:
		int intersects(
			const vl::vec2d& start_a,
			const vl::vec2d& end_a);

		bool contains(
			const vl::vec2d& min,
			const vl::vec2d& max,
			const vl::vec2d& v);

		std::vector<vl::vec2d> _vertices;
	};
}

