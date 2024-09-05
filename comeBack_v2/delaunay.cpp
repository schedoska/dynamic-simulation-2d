#include "delaunay.h"
#include <map>
#include <stack>

ds2::delaunay::delaunay(const std::vector<vl::vec2d>& vertices)
	: _vertices(vertices) {}

void ds2::delaunay::edge_flip(std::vector<triangle>& tr_l)
{
	std::map<segment, bool> marked;		// standarized index order
	std::stack<segment> to_check;		// standarized index order

	for (auto& tr : tr_l) {
		for (size_t i = 0; i < 3; ++i) {
			segment s(tr[i], tr[(i + 1) % 3]);
			std_seg(s);
			if (marked.count(s) == 0) {
				marked[s] = true;
				to_check.push(s);
			}
		}
	}

	while (!to_check.empty())
	{
		segment s = to_check.top();
		to_check.pop();
		marked[s] = false;

		// vector of found adj to edge triangles:
		// first	-> index of triangle
		// second	-> index of the opposite vertex
		std::vector<std::pair<size_t, size_t>> adj;

		for (size_t t_id = 0; t_id < tr_l.size(); ++t_id) {
			std::pair<bool, size_t> cs = contains_segment(tr_l[t_id], s);
			if (cs.first == false) continue;

			adj.push_back({ t_id, cs.second });
			if (adj.size() == 2) break;
		}
		if (adj.size() != 2) continue;

		triangle& tr_a = tr_l[adj[0].first];	// triangle a;
		triangle& tr_b = tr_l[adj[1].first];	// triangle b;
		const size_t tr_a_v = adj[0].second;	// local index of opposite vertex a
		const size_t tr_b_v = adj[1].second;	// local index of opposite vertex b

		if (is_legal(tr_a, tr_a_v, tr_b[tr_b_v])) continue;
		triangle new_tr_a = { s.first,  tr_b[tr_b_v], tr_a[tr_a_v] };
		triangle new_tr_b = { s.second, tr_a[tr_a_v], tr_b[tr_b_v] };

		auto push_to_check = [&](size_t& a, size_t& b)
		{
			segment s_ch = { a, b };
			std_seg(s_ch);
			if (marked[s_ch] == true) return;
			to_check.push(s_ch);
			marked[s_ch] = true;
		};
		push_to_check(s.first,  tr_b[tr_b_v]);
		push_to_check(s.first,  tr_a[tr_a_v]);
		push_to_check(s.second, tr_b[tr_b_v]);
		push_to_check(s.second, tr_a[tr_a_v]);

		tr_a = std::move(new_tr_a);
		tr_b = std::move(new_tr_b);
	}
}

/* Convert segment to standirezed order - smaller index first */
ds2::delaunay::segment& ds2::delaunay::std_seg(segment& s) const
{
	if (s.second < s.first) std::swap(s.first, s.second);
	return s;
}

/* return true if segment found as a part of triangle and local triangle 
   index with the opposite vertex */
std::pair<bool, size_t> ds2::delaunay::contains_segment(const triangle& tr, const segment& s) const
{
	for (int i = 0; i < 3; ++i) {
		segment i_s(tr[i], tr[(i + 1) % 3]);
		if (std_seg(i_s) == s) 
			return { true, (i + 2) % 3 };
	}
	return { false, 0 };
}

bool ds2::delaunay::is_legal(const triangle& tr, const size_t& b_loc, const size_t& d_id)
{
	const std::vector<vl::vec2d>& vl = _vertices;
	vl::vec2d B = vl[tr[b_loc]];
	vl::vec2d A = vl[tr[(b_loc + 1) % 3]];
	vl::vec2d C = vl[tr[(b_loc + 2) % 3]];
	vl::vec2d D = vl[d_id];

	auto x = [](const vl::vec2d& v) {return v[0]; };
	auto y = [](const vl::vec2d& v) {return v[1]; };

	double a = x(A) - x(D);
	double b = y(A) - y(D);
	double c = std::pow(x(A) - x(D), 2) + std::pow(y(A) - y(D), 2);
	double d = x(B) - x(D);
	double e = y(B) - y(D);
	double f = std::pow(x(B) - x(D), 2) + std::pow(y(B) - y(D), 2);
	double g = x(C) - x(D);
	double h = y(C) - y(D);
	double i = std::pow(x(C) - x(D), 2) + std::pow(y(C) - y(D), 2);

	double det = a * (e * i - f * h) - b * (d * i - g * f) + c * (d * h - e * g);
	double clck = vl::cross(B - A, C - A);
	return clck * det < 0 ? true : false;
}

