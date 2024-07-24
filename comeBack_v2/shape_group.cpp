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