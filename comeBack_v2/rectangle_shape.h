#pragma once

#include "convex_shape.h"

namespace ds2
{
	class rectangle_shape : public convex_shape
	{
	public:
		rectangle_shape(const vl::vec2d& size);
		const vl::vec2d& size() const;
		void set_size(const vl::vec2d& size);

	private:
		using convex_shape::add;
		using convex_shape::clear;
	};
}
