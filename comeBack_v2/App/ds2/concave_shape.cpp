#include "concave_shape.h"
#include "delaunay.h"
#include <queue>
#include <vector>

#define assertm(exp, msg) assert(((void)msg, exp))

void ds2::concave_shape::add(const vl::vec2d& v)
{
	_vertices.push_back(v);
}

void ds2::concave_shape::clear()
{
	_vertices.clear();
}

ds2::shape_group ds2::concave_shape::generate_shape_group(triangulation mode)
{
	const std::vector<vl::vec2d>& vl = _vertices;
	std::vector<triangle> tr_l = triangulate();

	if (mode == triangulation::delaunay) {
		delaunay del(vl);
		del.edge_flip(tr_l);
		make_clockwise(tr_l);
	}

	shape_group sg;

	for (const auto& i : tr_l){
		convex_shape tr;
		tr.add(vl[i[0]]);
		tr.add(vl[i[1]]);
		tr.add(vl[i[2]]);
		sg.add(tr);
	}

	return sg;
}

const std::vector<vl::vec2d>& ds2::concave_shape::vertices() const
{
	return _vertices;
}

std::vector<ds2::concave_shape::triangle> ds2::concave_shape::triangulate()
{
	//shape_group sg;
	std::vector<triangle> tr_l;
	const std::vector<vl::vec2d>& vl = _vertices;
	size_t size = vl.size();

	assertm(size > 2, "3 or more vertices are needed to generate concave shape!");

	std::vector<segment> ins_seg;
	std::vector<segment> out_seg;
	for (int i = 0; i < size; ++i) {
		out_seg.push_back({ i, (i + 1) % size });
	}

	auto loop = [](int a, int n) { return ((a % n) + n) % n; };

	std::queue<segment> to_check;
	segment s(0, 1);
	to_check.push(s);

	while (!to_check.empty())
	{
		segment base = to_check.front();
		to_check.pop();

		size_t b = base.first;
		size_t b_b1 = loop(b - 1, size);
		size_t b_f1 = loop(b + 1, size);

		size_t l = base.second;
		size_t l_b1 = loop(l - 1, size);
		size_t l_f1 = loop(l + 1, size);

		for (size_t c = 0; c < size; ++c)
		{
			if (utils::cross(vl[l] - vl[b], vl[c] - vl[b]) <= 0)
				continue;
			if (!contains(vl[b_f1] - vl[b], vl[b_b1] - vl[b], vl[c] - vl[b]))
				continue;
			if (!contains(vl[l_f1] - vl[l], vl[l_b1] - vl[l], vl[c] - vl[l]))
				continue;

			utils::segments_relation sr_b = intersects(out_seg, { b, c });
			utils::segments_relation sr_l = intersects(out_seg, { l, c });
			if (sr_b == utils::intetsects)
				continue;
			if (sr_l == utils::intetsects)
				continue;

			if (intersects(ins_seg, { b, c }) == utils::intetsects)
				continue;
			if (intersects(ins_seg, { l, c }) == utils::intetsects)
				continue;

			//convex_shape tr;
			//tr.add(vl[b]); tr.add(vl[l]); tr.add(vl[c]);
			//sg.add(tr);
			triangle tr = { b,l,c };
			tr_l.push_back(tr);

			if (sr_b != utils::same) {
				ins_seg.push_back({ b,c });
				to_check.push({ b,c });
			}
			if (sr_l != utils::same) {
				ins_seg.push_back({ c,l });
				to_check.push({ c,l });
			}
		}
	}
	return tr_l;
}

void ds2::concave_shape::make_clockwise(std::vector<triangle>& tr_l)
{
	const std::vector<vl::vec2d>& vl = _vertices;
	for (auto& tr : tr_l) {
		double a = vl::cross(vl[tr[1]] - vl[tr[0]], vl[tr[2]] - vl[tr[0]]);
		if (a < 0) std::swap(tr[1], tr[2]);
	}
}

utils::segments_relation ds2::concave_shape::intersects(
	const std::vector<segment>& s_list, 
	const segment& s)
{
	const std::vector<vl::vec2d>& vl = _vertices;
	int count = 0;

	for (const auto& it : s_list) {
		utils::segments_relation sr;
		sr = utils::check_relation(vl[it.first], vl[it.second], vl[s.first], vl[s.second]);

		if (sr == utils::same) 
			return utils::same;
		if (sr == utils::intetsects)
			++count;
	}
	return count == 0 ? utils::none : utils::intetsects;
}

bool ds2::concave_shape::contains(
	const vl::vec2d& min, 
	const vl::vec2d& max, 
	const vl::vec2d& v)
{
	double r = utils::angle(min, max);
	double a = utils::angle(min, v);
	if (a > 4.0 * std::acos(0.0) - 0.001) a = 0;

	return a <= r;
}
