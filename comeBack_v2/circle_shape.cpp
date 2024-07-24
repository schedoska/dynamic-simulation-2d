#include "circle_shape.h"

ds2::circle_shape::circle_shape(const vl::vec2d& loc_pos, const double& radius)
    : _loc_pos(loc_pos), _radius(radius) {}


const double& ds2::circle_shape::radius() const
{
    return _radius;
}

double& ds2::circle_shape::radius()
{
    return _radius;
}

const vl::vec2d& ds2::circle_shape::loc_pos() const
{
    return _loc_pos;
}

vl::vec2d& ds2::circle_shape::loc_pos()
{
    return _loc_pos;
}