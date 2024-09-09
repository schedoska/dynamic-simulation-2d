#pragma once

#include "shape_group.h"
#include "Utils.h"
#include <array>

namespace ds2
{
	enum class triangulation { expanding, delaunay };
		
	class concave_shape
	{
		using segment	= std::pair<size_t, size_t>;	// indexes of _vertices
		using triangle	= std::array<size_t, 3>;		// indexes of _vertices

	public:
		void add(const vl::vec2d& v);
		void clear();
		shape_group generate_shape_group(triangulation mode);
		const std::vector<vl::vec2d>& vertices() const;

	private:
		std::vector<triangle> triangulate();
		void make_clockwise(std::vector<triangle>& tr_l);

		utils::segments_relation intersects(
			const std::vector<segment>& s_list,
			const segment& s);

		bool contains(
			const vl::vec2d& min,
			const vl::vec2d& max,
			const vl::vec2d& v);

		std::vector<vl::vec2d> _vertices;
	};
}

