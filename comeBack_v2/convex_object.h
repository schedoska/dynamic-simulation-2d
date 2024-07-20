#pragma once
#include "object.h"

#include <SFML/Graphics.hpp>

namespace ds2
{
	class convex_object : public object
	{
	public:
		convex_object(const vl::vec2d& pos = vl::vec2d());
		void add_vertex(const vl::vec2d& v_pos);
		void clear_vertices();
		const std::vector<vl::vec2d>& vertices() const;
		virtual void scale(const vl::vec2d& s);

	private:
		/* List of points described in object local coordinates(0, 0) here is equal
		   to current position of obiject in space */
		std::vector<vl::vec2d> _vertices;
	};
}

