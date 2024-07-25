#include "rectangle_shape.h"

ds2::rectangle_shape::rectangle_shape(const vl::vec2d& size)
{
	set_size(size);
}

const vl::vec2d& ds2::rectangle_shape::size() const
{
	return _vertices[2] - _vertices[0];
}

void ds2::rectangle_shape::set_size(const vl::vec2d& size)
{
	_vertices.clear();
	double w = size[0];
	double h = size[1];
	_vertices.push_back(vl::vec2d(-w / 2.f, -h / 2.f));
	_vertices.push_back(vl::vec2d(w / 2.f, -h / 2.f));
	_vertices.push_back(vl::vec2d(w / 2.f, h / 2.f));
	_vertices.push_back(vl::vec2d(-w / 2.f, h / 2.f));
}

