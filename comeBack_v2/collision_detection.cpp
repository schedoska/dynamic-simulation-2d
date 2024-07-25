#include "collision_detection.h"

ds2::collision_data::collision_data()
    : collides(false), cp_a(vl::vec2d()), cp_b(vl::vec2d()) {}

ds2::collision_data::collision_data(
    bool _collides,
    const vl::vec2d& _cp_a,
    const vl::vec2d& _cp_b)
    : collides(_collides), cp_a(_cp_a), cp_b(_cp_b) {}

double ds2::collision_data::dist() const
{
    return (cp_a - cp_b).len();
}


ds2::object_collision_data ds2::collision_detection::check(
    const std::shared_ptr<object>& a, 
    const std::shared_ptr<object>& b)
{
    const shape_group& a_sh = a->shape();
    const shape_group& b_sh = b->shape();

    collision_data cd_max(false, vl::vec2d(0, std::numeric_limits<double>::min()));

    for (const auto& a_conv : a_sh.convexes()) {
        for (const auto& b_conv : b_sh.convexes()) {
            collision_data cd = check_shape(a, b, a_conv, b_conv);
            if (cd.collides == false) continue;
            cd_max = cd.dist() > cd_max.dist() ? cd : cd_max;
        }
    }
    for (const auto& a_conv : a_sh.convexes()) {
        for (const auto& b_circ : b_sh.circles()) {
            collision_data cd = check_shape(a, b, a_conv, b_circ);
            std::swap(cd.cp_a, cd.cp_b);
            if (cd.collides == false) continue;
            cd_max = cd.dist() > cd_max.dist() ? cd : cd_max;
        }
    }
    for (const auto& a_circ : a_sh.circles()) {
        for (const auto& b_conv : b_sh.convexes()) {
            collision_data cd = check_shape(b, a, b_conv, a_circ);
            if (cd.collides == false) continue;
            cd_max = cd.dist() > cd_max.dist() ? cd : cd_max;
        }
    }
    for (const auto& a_circ : a_sh.circles()) {
        for (const auto& b_circ : b_sh.circles()) {
            collision_data cd = check_shape(b, a, b_circ, a_circ);
            if (cd.collides == false) continue;
            cd_max = cd.dist() > cd_max.dist() ? cd : cd_max;
        }
    }
    return object_collision_data(a, b, cd_max);
}

ds2::collision_data ds2::collision_detection::check_shape(
    const std::shared_ptr<object>& a, 
    const std::shared_ptr<object>& b, 
    const convex_shape& a_sh, 
    const circle_shape& b_sh)
{
    const std::vector<vl::vec2d>& a_list = a_sh.vertices();
    size_t a_size = a_list.size();

    double min_overlap = std::numeric_limits<double>::max();
    vl::vec2d cp_a, cp_b;
    vl::vec2d cn;

    /* Vector from center of circle to closoest convex shape vertex */
    vl::vec2d min_vec(0, std::numeric_limits<double>::max()); 

    for (int i = 0; i < a_size; ++i) {
        vl::vec2d edge = a->global(a_list[(i + 1) % a_size]) - a->global(a_list[i]);
        vl::vec2d axis = vl::vec2d(edge[1], -edge[0]).normalize();
        axis_projection_data apd = project_on_axis(a, b, a_sh, b_sh, axis);

        if (!apd.is_overlaping)
            return collision_data(false);
        if (apd.penetration < min_overlap) {
            min_overlap = apd.penetration;
            cn = apd.collision_normal;
            cp_b = apd.closest_point;
            cp_a = cp_b + cn * min_overlap;
        }
        vl::vec2d dv = b->global(b_sh.loc_pos()) - a->global(a_list[i]);
        min_vec = dv.len() < min_vec.len() ? dv : min_vec;
    }

    axis_projection_data apd = project_on_axis(a, b, a_sh, b_sh, min_vec.normalize());

    if (!apd.is_overlaping)
        return collision_data(false);
    if (apd.penetration < min_overlap) {
        min_overlap = apd.penetration;
        cn = apd.collision_normal;
        cp_b = apd.closest_point;
        cp_a = cp_b + cn * min_overlap;
    }
    
    return collision_data(true, cp_b, cp_a);
}

ds2::collision_data ds2::collision_detection::check_shape(
    const std::shared_ptr<object>& a,
    const std::shared_ptr<object>& b,
    const convex_shape& a_sh, 
    const convex_shape& b_sh)
{
    const std::vector<vl::vec2d>& a_list = a_sh.vertices();
    const std::vector<vl::vec2d>& b_list = b_sh.vertices();
    size_t a_size = a_list.size();
    size_t b_size = b_list.size();

    double min_overlap = std::numeric_limits<float>::max();
    vl::vec2d cpA, cpB;     /* Contact points   */
    vl::vec2d cn;           /* Collision normal */

    for (size_t i = 0; i < a_size; ++i) {
        vl::vec2d edge = a->global(a_list[(i + 1) % a_size]) - a->global(a_list[i]);
        vl::vec2d axis = vl::vec2d(edge[1], -edge[0]).normalize();
        axis_projection_data apd = project_on_axis(a, b, a_sh, b_sh, axis);
        if (apd.is_overlaping == false)
            return collision_data(false);

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
        axis_projection_data apd = project_on_axis(b, a, b_sh, a_sh, axis);
        if (apd.is_overlaping == false)
            return collision_data(false);

        if (apd.penetration <= min_overlap) {
            min_overlap = apd.penetration;
            cn = apd.collision_normal;
            cpA = apd.closest_point;
            cpB = cpA + cn * min_overlap;
        }
    }
    return collision_data(true, cpA, cpB);
}

ds2::collision_data ds2::collision_detection::check_shape(
    const std::shared_ptr<object>& a, 
    const std::shared_ptr<object>& b, 
    const circle_shape& a_sh, 
    const circle_shape& b_sh)
{
    if (a == b)
        return collision_data(false);

    vl::vec2d ab = a->global(a_sh.loc_pos()) - b->global(b_sh.loc_pos());
    double    rr = a_sh.radius() + b_sh.radius();
    if (ab.len() > rr)
        return collision_data(false);

    ab.normalize();
    vl::vec2d cp_a = ab * -a_sh.radius() + a->global(a_sh.loc_pos());
    vl::vec2d cp_b = ab *  b_sh.radius() + b->global(b_sh.loc_pos());
    return collision_data(true, cp_b, cp_a);
}

ds2::axis_projection_data ds2::collision_detection::project_on_axis(
    const std::shared_ptr<object>& ref, 
    const std::shared_ptr<object>& sec, 
    const convex_shape& ref_sh, 
    const convex_shape& sec_sh, 
    const vl::vec2d& axis)
{
    axis_projection_data apd;
    apd.collision_normal = axis;

    double max_ref = -std::numeric_limits<double>::max();  
    double min_ref = std::numeric_limits<double>::max();  
    for (const auto& v : ref_sh.vertices()) {
        double val = axis.dot(ref->global(v));
        max_ref = std::max(val, max_ref);
        min_ref = std::min(val, min_ref);
    }

    double max_sec = -std::numeric_limits<double>::max();  
    double min_sec = std::numeric_limits<double>::max();  
    vl::vec2d max_sec_vec, min_sec_vec;
    for (const auto& v : sec_sh.vertices()) {
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
    const std::shared_ptr<object>& ref, 
    const std::shared_ptr<object>& sec, 
    const convex_shape& ref_sh, 
    const circle_shape& sec_sh, 
    const vl::vec2d& axis)
{
    axis_projection_data apd;
    apd.collision_normal = axis;

    double max_ref = -std::numeric_limits<double>::max();  
    double min_ref = std::numeric_limits<double>::max();   
    for (const vl::vec2d& v : ref_sh.vertices()) {
        double val = axis.dot(ref->global(v));
        max_ref = std::max(val, max_ref);
        min_ref = std::min(val, min_ref);
    }

    double max_sec, min_sec;
    vl::vec2d max_sec_vec, min_sec_vec;
    vl::vec2d sec_1 = sec->global(sec_sh.loc_pos()) + axis * sec_sh.radius();
    vl::vec2d sec_2 = sec->global(sec_sh.loc_pos()) - axis * sec_sh.radius();
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
    }
    else if (min_sec > min_ref && max_sec < max_ref) {
        apd.penetration = std::abs(min_ref - max_sec);
        apd.closest_point = max_sec_vec;
        apd.collision_normal = apd.collision_normal * (-1);
    }
    else if (min_sec > min_ref) {
        apd.penetration = std::abs(max_ref - min_sec);
        apd.closest_point = min_sec_vec;
    }
    else {
        apd.penetration = std::abs(max_sec - min_ref);
        apd.closest_point = max_sec_vec;
        apd.collision_normal = apd.collision_normal * (-1);
    }
    return apd;
}

ds2::object_collision_data::object_collision_data(
    const std::weak_ptr<object>& _a,
    const std::weak_ptr<object>& _b,
    const collision_data& _collision_data)
    : a(_a), b(_b), data(_collision_data) {}
