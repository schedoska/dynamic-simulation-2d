#pragma once

#include "shape_group.h"

namespace ds2
{
	class delaunay
	{
		using segment = std::pair<size_t, size_t>;	// indexes of _vertices
		using triangle = std::array<size_t, 3>;		// indexes of _vertices

	public:
		delaunay(const std::vector<vl::vec2d>& vertices);
		void edge_flip(std::vector<triangle>& tr_l);

	private:
		const std::vector<vl::vec2d>& _vertices;

		segment& std_seg(segment& s) const;
		std::pair<bool, size_t> contains_segment(const triangle& tr, const segment& s) const;
		bool is_legal(const triangle& tr, const size_t& b_loc, const size_t& d);
	};
}

