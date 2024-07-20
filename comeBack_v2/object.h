#pragma once
#include "vec2.h"

namespace ds2
{
	class object {
	public:
		object();
		object(const vl::vec2d& pos);

		vl::vec2d& pos();
		const vl::vec2d& pos() const;
		vl::vec2d& vel();
		const vl::vec2d& vel() const;
		double& mass();
		const double& mass() const;
		double& rot();
		const double& rot() const;
		double& rot_vel();
		const double& rot_vel() const;

		virtual void update(const double& dt);
		vl::vec2d global(const vl::vec2d& local);
		vl::vec2d local (const vl::vec2d& global);

	private:
		void init();

		vl::vec2d _pos;
		vl::vec2d _vel;

		double _rot;
		double _rot_vel;
		double _mass;
	};
}
