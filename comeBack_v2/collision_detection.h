#pragma once

#include "convex_object.h"
#include "circle_object.h"

namespace ds2
{
	struct collision_data
	{
		collision_data(
			bool _collides,
			const std::weak_ptr<object>& _a,
			const std::weak_ptr<object>& _b,
			const vl::vec2d& _cp_a = vl::vec2d(),
			const vl::vec2d& _cp_b = vl::vec2d());

		bool collides;
		std::weak_ptr<object> a;
		std::weak_ptr<object> b;
		vl::vec2d cp_a;
		vl::vec2d cp_b;
	};

	class collision_detection
	{
	public:
		static collision_data check(
			const std::shared_ptr<circle_object>& a, 
			const std::shared_ptr<circle_object>& b);
	};
}

