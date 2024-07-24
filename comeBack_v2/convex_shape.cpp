#include "convex_shape.h"

ds2::convex_shape::convex_shape() {}

void ds2::convex_shape::add(const vl::vec2d& vertex)
{
    _vertices.push_back(vertex);
}

void ds2::convex_shape::clear()
{
    _vertices.clear();
}

const std::vector<vl::vec2d>& ds2::convex_shape::vertices() const
{
    return _vertices;
}

void ds2::convex_shape::translate(const vl::vec2d& v)
{
    for (vl::vec2d& i : _vertices) i += v;
}