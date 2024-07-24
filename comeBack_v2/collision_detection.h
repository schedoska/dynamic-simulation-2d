#pragma once

#include "convex_object.h"
#include "circle_object.h"

namespace ds2
{
	struct axis_projection_data
	{
		axis_projection_data();
		bool is_overlaping;
		float penetration;
		vl::vec2d closest_point;	/* Secondary object */
		vl::vec2d collision_normal;
	};

	struct collision_data_2
	{
		collision_data_2();
		collision_data_2(
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
			const std::weak_ptr<object>& a,
			const std::weak_ptr<object>& b,
			const collision_data_2& collision_data = collision_data_2());

		collision_data_2 data;
		std::weak_ptr<object> a;
		std::weak_ptr<object> b;
	};

	class collision_detection_2
	{
	public:
		static object_collision_data check(
			const std::shared_ptr<object>& a,
			const std::shared_ptr<object>& b);

	private:
		static collision_data_2 check_shape(
			const std::shared_ptr<object>& a,
			const std::shared_ptr<object>& b,
			const convex_shape& a_sh,
			const circle_shape& b_sh);

		static collision_data_2 check_shape(
			const std::shared_ptr<object>& a,
			const std::shared_ptr<object>& b,
			const convex_shape& a_sh,
			const convex_shape& b_sh);

		static collision_data_2 check_shape(
			const std::shared_ptr<object>& a,
			const std::shared_ptr<object>& b,
			const circle_shape& a_sh,
			const circle_shape& b_sh);

		static axis_projection_data project_on_axis(
			const std::shared_ptr<object>& ref,
			const std::shared_ptr<object>& sec,
			const convex_shape& ref_sh,
			const convex_shape& sec_sh,
			const vl::vec2d& axis);

		static axis_projection_data project_on_axis(
			const std::shared_ptr<object>& ref,
			const std::shared_ptr<object>& sec,
			const convex_shape& ref_sh,
			const circle_shape& sec_sh,
			const vl::vec2d& axis);
	};
}

