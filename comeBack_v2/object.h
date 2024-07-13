#pragma once
#include "vec2.h"

namespace ds2
{
	class object {
	public:
		object();
		object(const vl::vec2d& pos);
		vl::vec2d&			pos();
		const vl::vec2d&	pos() const;
		vl::vec2d&			vel();
		const vl::vec2d&	vel() const;
		double& mass();
		const double& mass() const;
		void update(const double& dt);

	private:
		vl::vec2d _pos;
		vl::vec2d _vel;
		double _mass;
	};
}
