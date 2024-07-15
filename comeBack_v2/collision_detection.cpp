#include "collision_detection.h"

ds2::collision_data::collision_data(
	bool _collides,
	const std::weak_ptr<object>& _a,
	const std::weak_ptr<object>& _b,
	const vl::vec2d& _cp_a,
	const vl::vec2d& _cp_b
) :
	collides(_collides),
	a(_a),
	b(_b),
	cp_a(_cp_a),
	cp_b(_cp_b) 
{}

ds2::collision_data ds2::collision_detection::check(
	const std::shared_ptr<circle_object>& a, 
	const std::shared_ptr<circle_object>& b)
{
	if (a == b) 
		return collision_data(false, a, b);

	vl::vec2d ab = a->pos() - b->pos();
	double    rr = a->radius() + b->radius();
	if (ab.len() > rr)
		return collision_data(false, a, b);

	ab.normalize();
	vl::vec2d cp_a = ab * -a->radius();
	vl::vec2d cp_b = ab *  b->radius();
	return collision_data(true, a, b, cp_a, cp_b);
}
