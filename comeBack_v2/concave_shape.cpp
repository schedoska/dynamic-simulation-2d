#include "concave_shape.h"
#include "Utils.h"

void ds2::concave_shape::add(const vl::vec2d& v)
{
	_vertices.push_back(v);
}

void ds2::concave_shape::clear()
{
	_vertices.clear();
}

ds2::shape_group ds2::concave_shape::generate_group()
{
	shape_group sg;
	const std::vector<vl::vec2d>& vl = _vertices;
	size_t size = vl.size();
	int f_id = 2;
	int b_id = size;

	bool f_turn = true;
	int l = 0;	// linker index
	int b = 1;	// back index
	int c = 2;	// candidate index

	auto loop = [](auto a, auto n) { return ((a % n) + n) % n; };

	while( true )
	{
		size_t lb1 = loop(l - 1, size);		// 1 back to linker
		size_t lf1 = loop(l + 1, size);		// 1 forward to linker
		size_t cb1 = loop(c - 1, size);		// 1 back to candidate 
		size_t cf1 = loop(c + 1, size);		// 1 forward to candidate 

		if (b_id - f_id < 2) {
			convex_shape tr;
			tr.add(vl[l]); tr.add(vl[b]); tr.add(vl[c]);
			std::cout << "DODAJE: " << vl[l] << " " << vl[b] << " " << vl[c] << "\n";
			sg.add(tr);
			break;
		}

		if (contains(vl[lf1] - vl[l], vl[lb1] - vl[l], vl[c] - vl[l]) &&
			contains(vl[cf1] - vl[c], vl[cb1] - vl[c], vl[l] - vl[c]) &&
			intersects(vl[c], vl[l]) == 4) 
		{
			convex_shape tr;
			tr.add(vl[l]); tr.add(vl[b]); tr.add(vl[c]);
			sg.add(tr);
			b = l;
			l = c;
			c = f_turn ? --b_id : ++f_id;
			f_turn = !f_turn;
		}
		else 
		{
			if (f_turn) { c = --b_id; --f_id; }
			else { c = ++f_id; ++b_id; }
			f_turn = !f_turn;
			std::swap(b, l);
		}
	}
	return sg;
}

const std::vector<vl::vec2d>& ds2::concave_shape::vertices() const
{
	return _vertices;
}

int ds2::concave_shape::intersects(
	const vl::vec2d& start_a, 
	const vl::vec2d& end_a)
{
	int count = 0;
	size_t s = _vertices.size();
	for (size_t i = 0; i < s; ++i) {
		if (utils::intersects(
			start_a, end_a, _vertices[i], _vertices[(i + 1) % s])) count++;
	}
	return count;
}

bool ds2::concave_shape::contains(
	const vl::vec2d& min, 
	const vl::vec2d& max, 
	const vl::vec2d& v)
{
	double r = utils::angle(min, max);
	double a = utils::angle(min, v);
	return a < r;
}
