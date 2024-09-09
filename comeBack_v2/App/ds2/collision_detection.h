#pragma once

#include "object.h"
#include "axis_projection_data.h"

namespace ds2
{
	struct collision_data
	{
		collision_data();
		collision_data(
			bool collides, 
			const vl::vec2d& cp_a = vl::vec2d(), 
			const vl::vec2d& cp_b = vl::vec2d());
		double dist() const;

		bool collides;
		vl::vec2d cp_a;		/* global contact point position a */
		vl::vec2d cp_b;		/* global contact point position b */
	};

	struct object_collision_data
	{
		object_collision_data(
			object* a,
			object* b,
			const collision_data& collision_data = collision_data());

		collision_data data;
		object* a;
		object* b;
	};

	class collision_detection
	{
	public:
		static object_collision_data check(
			object* a,
			object* b);

	private:
		static collision_data check_shape(
			object* a,
			object* b,
			const convex_shape& a_sh,
			const circle_shape& b_sh);

		static collision_data check_shape(
			object* a,
			object* b,
			const convex_shape& a_sh,
			const convex_shape& b_sh);

		static collision_data check_shape(
			object* a,
			object* b,
			const circle_shape& a_sh,
			const circle_shape& b_sh);

		static axis_projection_data project_on_axis(
			const object* ref,
			const object* sec,
			const convex_shape& ref_sh,
			const convex_shape& sec_sh,
			const vl::vec2d& axis);

		static axis_projection_data project_on_axis(
			const object* ref,
			const object* sec,
			const convex_shape& ref_sh,
			const circle_shape& sec_sh,
			const vl::vec2d& axis);
	};
}

