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
		vl::vec2d& acc();
		const vl::vec2d& acc() const;
		double& mass();
		const double& mass() const;
		double& rot();
		const double& rot() const;
		double& rot_vel();
		const double& rot_vel() const;
		double& rot_acc();
		const double& rot_acc() const;

		virtual void update(const double& dt);
		vl::vec2d global(const vl::vec2d& local);
		vl::vec2d local (const vl::vec2d& global);
		void apply_force(const vl::vec2d& force, const vl::vec2d& point, const double& dt);

		double inertia;

	private:
		void init();

		vl::vec2d _pos;
		vl::vec2d _vel;

		vl::vec2d _acc;
		double _rot_acc;

		double _rot;
		double _rot_vel;
		double _mass;
	};
}
