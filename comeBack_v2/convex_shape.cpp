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

double ds2::convex_shape::area() const
{
    size_t s = _vertices.size();
    double a(0.0);
    for (size_t i = 0; i < s; ++i) {
        const vl::vec2d& v_0 = _vertices[i];
        const vl::vec2d& v_1 = _vertices[(i + 1) % s];
        a += (v_0[0] * v_1[1] - v_1[0] * v_0[1]);
    }
    return a / 2.0;
}

vl::vec2d ds2::convex_shape::centroid() const
{
    size_t s = _vertices.size();
    double c_x(0.0), c_y(0.0);
    for (size_t i = 0; i < s; ++i) {
        const vl::vec2d& v_0 = _vertices[i];
        const vl::vec2d& v_1 = _vertices[(i + 1) % s];
        c_x += (v_0[0] + v_1[0]) * (v_0[0] * v_1[1] - v_1[0] * v_0[1]);
        c_y += (v_0[1] + v_1[1]) * (v_0[0] * v_1[1] - v_1[0] * v_0[1]);
    }
    double a_6 = area() * 6;
    return vl::vec2d(c_x / a_6, c_y / a_6);
}

double ds2::convex_shape::second_moment_area() const
{
    size_t s = _vertices.size();
    double j_x(0.0), j_y(0.0);
    for (size_t i = 0; i < s; ++i) {
        const vl::vec2d& v_0 = _vertices[i];
        const vl::vec2d& v_1 = _vertices[(i + 1) % s];
        j_x += (v_0[0] * v_1[1] - v_1[0] * v_0[1])
            * (std::pow(v_0[1], 2) + v_0[1] * v_1[1] + std::pow(v_1[1], 2));
        j_y += (v_0[0] * v_1[1] - v_1[0] * v_0[1])
            * (std::pow(v_0[0], 2) + v_0[0] * v_1[0] + std::pow(v_1[0], 2));
    }
    return ((j_x + j_y) / 12.0);
}

const ds2::rect ds2::convex_shape::box(const vl::vec2d& pos, const double& rot) const
{
    constexpr double dmax = std::numeric_limits<double>::max();
    rect box = { {dmax, dmax}, -dmax, -dmax };

    for (auto& i : _vertices) {
        box.expand(vl::rotate(i, rot) + pos);
    }
    return box;
}

