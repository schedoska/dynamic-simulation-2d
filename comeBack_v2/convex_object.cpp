#include "convex_object.h"

ds2::convex_object::convex_object(const vl::vec2d& pos) :
	object(pos) {
}

void ds2::convex_object::add_vertex(const vl::vec2d& v_pos) {
	_vertices.push_back(v_pos);
}

void ds2::convex_object::clear_vertices()
{
	_vertices.clear();
}

const std::vector<vl::vec2d>& ds2::convex_object::vertices() const
{
	return _vertices;
}

void ds2::convex_object::scale(const vl::vec2d& s)
{
	for (auto& v : _vertices) {
		v[0] *= s[0];
		v[1] *= s[1];
	}
}
