#include "shape.h"

ds2::rect::rect()
	:w(0.0), h(0.0) {}

ds2::rect::rect(const vl::vec2d& _pos, const double& _w, const double& _h)
	: pos(_pos), w(_w), h(_h) {}

void ds2::rect::expand(const vl::vec2d& v)
{
	if (v[0] < pos[0]) {
		w += (pos[0] - v[0]);
		pos[0] = v[0];
	}
	else if (v[0] > pos[0] + w)
		w = v[0] - pos[0];

	if (v[1] < pos[1]) {
		h += (pos[1] - v[1]);
		pos[1] = v[1];
	}
	else if (v[1] > pos[1] + h)
		h = v[1] - pos[1];
}

void ds2::rect::expand(const rect& _rect)
{
	expand(_rect.pos);
	expand(_rect.pos + vl::vec2d(_rect.w, _rect.h));
}

const ds2::rect ds2::shape::box(const vl::vec2d& pos, const double& rot) const
{
	return rect();
}
