#include "delaunay.h"
#include <map>
#include <stack>

ds2::delaunay::delaunay(const std::vector<vl::vec2d>& vertices)
	: _vertices(vertices)
{
}

void ds2::delaunay::edge_flip(std::vector<triangle>& tr_l)
{
	std::cout << tr_l.size() << "\n";

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

	for (int qq = 0; qq < 14; qq++)
	{
		segment s = to_check.top();
		to_check.pop();

		// vector of found adj to edge triangles :
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

		const triangle& tr_a = tr_l[adj[0].first];	// triangle a;
		const triangle& tr_b = tr_l[adj[1].first];	// triangle b;
		const size_t tr_a_v = adj[0].second;		// local index of opposite vertex a
		const size_t tr_b_v = adj[1].second;		// local index of opposite vertex b

		std::cout << "EDGE: " << s.first << " " << s.second << "\n";
		std::cout << "TRIANGLE_1: " << tr_a[0] << " " << tr_a[1] << " " << tr_a[2] << "\n";
		std::cout << "TRIANGLE_2: " << tr_b[0] << " " << tr_b[1] << " " << tr_b[2] << "\n";
		std::cout << "TR_1 oppo: " << tr_a[tr_a_v] << "\n";
		std::cout << "TR_2 oppo: " << tr_b[tr_b_v] << "\n";
		bool ass=is_legal(tr_a, tr_a_v, tr_b[tr_b_v]);
		//is_legal(tr_b, tr_b_v, tr_a[tr_a_v]);
		std::cout << ass << "\n\n";

	}






	std::cout << to_check.size() << "\n";
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

	std::cout << tr[b_loc] << " " << tr[(b_loc + 1) % 3] << " " << tr[(b_loc + 2) % 3] << "\n";
	std::cout << "D: " << d_id << "\n";

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

