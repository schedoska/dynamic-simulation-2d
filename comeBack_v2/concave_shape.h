#pragma once

#include "shape_group.h"
#include "Utils.h"

namespace ds2
{
	using segment = std::pair<size_t, size_t>;	// indexes of vertices

	class concave_shape
	{
	public:
		void add(const vl::vec2d& v);
		void clear();
		shape_group generate_group();
		const std::vector<vl::vec2d>& vertices() const;

	//private:
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

