#include "shape_group.h"

void ds2::shape_group::add(const circle_shape& circle)
{
    _circles.push_back(circle);
}

void ds2::shape_group::add(const convex_shape& convex)
{
    _convexes.push_back(convex);
}

const std::vector<ds2::circle_shape>& ds2::shape_group::circles() const
{
    return _circles;
}

const std::vector<ds2::convex_shape>& ds2::shape_group::convexes() const
{
    return _convexes;
}

void ds2::shape_group::clear()
{
    _circles.clear();
    _convexes.clear();
}

void ds2::shape_group::translate(const vl::vec2d& v)
{
    for (auto& i : _convexes)
        i.translate(v);
    for (auto& i : _circles)
        i.loc_pos() += v;
}

void ds2::shape_group::translate_to_centroid()
{
    translate(centroid() * -1.0);
}

double ds2::shape_group::area() const
{
    double a(0.0);
    for (const auto& i : _convexes) a += i.area();
    for (const auto& i : _circles ) a += i.area();
    return a;
}

vl::vec2d ds2::shape_group::centroid() const
{
    vl::vec2d c_m = { 0,0 };
    for (const auto& i : _convexes)
        c_m += i.centroid() * i.area();
    for (const auto& i : _circles)
        c_m += i.centroid() * i.area();

    const double a = area();
    return c_m / a;
}

double ds2::shape_group::second_moment_area() const
{
    double j_z(0.0);
    for (const auto& i : _convexes) {
        double d = i.centroid().len();
        j_z += i.second_moment_area();
    }
    for (const auto& i : _circles) {
        double d = i.centroid().len();
        j_z += i.second_moment_area();
    }
    return j_z;
}
