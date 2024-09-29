#include "circle_shape.h"

ds2::circle_shape::circle_shape(const vl::vec2d& loc_pos, const double& radius)
    : _loc_pos(loc_pos), _radius(radius) 
{
   
}


const double& ds2::circle_shape::radius() const
{
    return _radius;
}

void ds2::circle_shape::set_radius(const double& radius)
{
    _radius = radius;
}

const vl::vec2d& ds2::circle_shape::loc_pos() const
{
    return _loc_pos;
}

void ds2::circle_shape::set_loc_pos(const vl::vec2d& pos)
{
    _loc_pos = pos;
}

double ds2::circle_shape::area() const
{
    return std::acos(-1) * std::pow(_radius, 2);
}

double ds2::circle_shape::second_moment_area() const
{
    const double d = _loc_pos.len();
    return std::acos(-1) * std::pow(_radius, 4) / 2.0 + std::pow(d, 2) * area();
}

const ds2::rect ds2::circle_shape::box(const vl::vec2d& pos, const double& rot) const
{
    vl::vec2d cp = vl::rotate(_loc_pos, rot) + pos;
    rect box = { cp - vl::vec2d(_radius, _radius) , 2.0 * _radius, 2.0 * _radius };
    return box;
}

vl::vec2d ds2::circle_shape::centroid() const
{
    return _loc_pos;
}

void ds2::circle_shape::scale(const vl::vec2d& scale)
{
    _loc_pos[0] *= scale[0];
    _loc_pos[1] *= scale[1];
    _radius *= std::max(scale[0], scale[1]);
}

void ds2::circle_shape::rotate(const double& rad)
{
    _loc_pos = vl::rotate(_loc_pos, rad);
}

