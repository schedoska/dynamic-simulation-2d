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
	vl::vec2d cp_a = ab * -a->radius() + a->pos();
	vl::vec2d cp_b = ab *  b->radius() + b->pos();
	return collision_data(true, a, b, cp_a, cp_b);
}

ds2::collision_data ds2::collision_detection::check(
    const std::shared_ptr<convex_object>& a,
    const std::shared_ptr<circle_object>& b)
{
    const std::vector<vl::vec2d>& a_list = a->vertices();
    size_t a_size = a_list.size();

    double min_overlap = std::numeric_limits<double>::max();
    vl::vec2d cp_a, cp_b;
    vl::vec2d cn;

    for (int i = 0; i < a_size; ++i) {
        vl::vec2d edge = a->global(a_list[(i + 1) % a_size]) - a->global(a_list[i]);
        vl::vec2d axis = vl::vec2d(edge[1], -edge[0]).normalize();
        axis_projection_data apd = project_on_axis(a, b, axis);

        if (!apd.is_overlaping)
            return collision_data(false, a, b);

        if (apd.penetration < min_overlap) {
            min_overlap = apd.penetration;
            cn = apd.collision_normal;
            cp_b = apd.closest_point;
            cp_a = cp_b + cn * min_overlap;
        }
    }
    for (int i = 0; i < a_size; ++i) {
        vl::vec2d edge = a->global(a_list[(i + 1) % a_size]) - a->global(a_list[i]);
        axis_projection_data apd = project_on_axis(a, b, edge.normalize());

        if (!apd.is_overlaping)
            return collision_data(false, a, b);

        if (apd.penetration < min_overlap) {
            min_overlap = apd.penetration;
            cn = apd.collision_normal;
            cp_b = apd.closest_point;
            cp_a = cp_b + cn * min_overlap;
        }
    }
    return collision_data(true, a, b, cp_a, cp_b);
}

ds2::collision_data ds2::collision_detection::check(
	const std::shared_ptr<convex_object>& a,
	const std::shared_ptr<convex_object>& b)
{
	
    if (a == b) return collision_data(false, a, b);

    const std::vector<vl::vec2d>& a_list = a->vertices();
    const std::vector<vl::vec2d>& b_list = b->vertices();
    size_t a_size = a_list.size();
    size_t b_size = b_list.size();

    double min_overlap = std::numeric_limits<float>::max();
    vl::vec2d cpA, cpB;     /* Contact points */
    vl::vec2d cn;           /* Collision normal */

    for (size_t i = 0; i < a_size; ++i) {
        vl::vec2d edge = a->global(a_list[(i + 1) % a_size]) - a->global(a_list[i]);
        vl::vec2d axis = vl::vec2d(edge[1], -edge[0]).normalize();
        axis_projection_data apd = project_on_axis(a, b, axis);
        if (apd.is_overlaping == false)
            return collision_data(false, a, b);

        if (apd.penetration <= min_overlap) {
            min_overlap = apd.penetration;
            cn = apd.collision_normal;
            cpB = apd.closest_point;
            cpA = cpB + cn * min_overlap;
        }
    }

    for (size_t i = 0; i < b_size; ++i) {
        vl::vec2d edge = b->global(b_list[(i + 1) % b_size]) - b->global(b_list[i]);
        vl::vec2d axis = vl::vec2d(edge[1], -edge[0]).normalize();
        axis_projection_data apd = project_on_axis(b, a, axis);
        if (!apd.is_overlaping)
            return collision_data(false, a, b);

        if (apd.penetration <= min_overlap) {
            min_overlap = apd.penetration;
            cn = apd.collision_normal;
            cpA = apd.closest_point;
            cpB = cpA + cn * min_overlap;
        }
    }
    return collision_data(true, a, b, cpA, cpB);
}

ds2::axis_projection_data ds2::collision_detection::project_on_axis(
	const std::shared_ptr<convex_object>& ref, 
	const std::shared_ptr<convex_object>& sec, 
	const vl::vec2d& axis)
{
    axis_projection_data apd;
    apd.collision_normal = axis;

    double max_ref = -std::numeric_limits<double>::max();  //usun
    double min_ref = std::numeric_limits<double>::max();  //usun
    for (const auto& v : ref->vertices()) {
        double val = axis.dot(ref->global(v));
        max_ref = std::max(val, max_ref);
        min_ref = std::min(val, min_ref);
    }

    double max_sec = -std::numeric_limits<double>::max();  //usun
    double min_sec = std::numeric_limits<double>::max();  //usun
    vl::vec2d max_sec_vec, min_sec_vec;
    for (const auto& v : sec->vertices()) {
        vl::vec2d v_glob = sec->global(v);
        double val = axis.dot(v_glob);
        if (val > max_sec) {
            max_sec = val;
            max_sec_vec = v_glob;
        }
        if (val < min_sec) {
            min_sec = val;
            min_sec_vec = v_glob;
        }
    }
    /* Check if Collision possibly occured */
    if (min_sec > max_ref ||
        max_sec < min_ref) {
        apd.is_overlaping = false;
        return apd;
    }
    /* Calculate overlapping value and Determine closest secondary Point to the referenc object */
    if (min_sec > min_ref && max_sec < max_ref) {
        apd.penetration = std::abs(min_ref - max_sec);
        apd.closest_point = max_sec_vec;
        apd.collision_normal = apd.collision_normal * (-1.f);
    }
    else if (min_sec > min_ref) {
        apd.penetration = std::abs(max_ref - min_sec);
        apd.closest_point = min_sec_vec;
    }
    else {
        apd.penetration = std::abs(max_sec - min_ref);
        apd.closest_point = max_sec_vec;
        apd.collision_normal = apd.collision_normal * (-1.f);
    }
    return apd;
}

ds2::axis_projection_data ds2::collision_detection::project_on_axis(
	const std::shared_ptr<convex_object>& ref,
	const std::shared_ptr<circle_object>& sec,
	const vl::vec2d& axis)
{
    axis_projection_data apd;
    apd.collision_normal = axis;

    double max_ref = -std::numeric_limits<double>::max();  //usun
    double min_ref = std::numeric_limits<double>::max();   //usun
    for (const vl::vec2d &v : ref->vertices()) {
        double val = axis.dot(ref->global(v));
        max_ref = std::max(val, max_ref);
        min_ref = std::min(val, min_ref);
    }

    double max_sec, min_sec;
    vl::vec2d max_sec_vec, min_sec_vec;
    vl::vec2d sec_1 = sec->pos() + axis * sec->radius();
    vl::vec2d sec_2 = sec->pos() - axis * sec->radius();
    double val_1 = axis.dot(sec_1);
    double val_2 = axis.dot(sec_2);

    if (val_1 > val_2) {
        max_sec = val_1;
        min_sec = val_2;
        max_sec_vec = sec_1;
        min_sec_vec = sec_2;
    }
    else {
        max_sec = val_2;
        min_sec = val_1;
        max_sec_vec = sec_2;
        min_sec_vec = sec_1;
    }
    /* Check if Collision did not occured */
    if (min_sec > max_ref ||
        max_sec < min_ref) {
        apd.is_overlaping = false;
        return apd;
    }
    /* Calculate overlapping value and Determine closest secondary Point to the referenc object */
    if (min_ref > min_sec && max_ref < max_sec) {
        apd.penetration = std::abs(min_ref - max_sec);
        apd.closest_point = max_sec_vec;
        apd.collision_normal = apd.collision_normal * (-1);
        //std::cout << " HELLOS 1 : " << axis << "\n";
    }
    else if (min_sec > min_ref && max_sec < max_ref) {
        apd.penetration = std::abs(min_ref - max_sec);
        apd.closest_point = max_sec_vec;
        apd.collision_normal = apd.collision_normal * (-1);
    }
    else if (min_sec > min_ref) {
        apd.penetration = std::abs(max_ref - min_sec);
        apd.closest_point = min_sec_vec;
        //std::cout << " HELLOS 2 : " << axis << "\n";
    }
    else {
        apd.penetration = std::abs(max_sec - min_ref);
        apd.closest_point = max_sec_vec;
        apd.collision_normal = apd.collision_normal * (-1);
    }
    return apd;
}

ds2::axis_projection_data::axis_projection_data()
{
    is_overlaping = true;
}
