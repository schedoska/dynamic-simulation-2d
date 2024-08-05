#pragma once

#include "shape_group.h"

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
		const double& mass() const;
		double& rot();
		const double& rot() const;
		double& rot_vel();
		const double& rot_vel() const;
		double& inertia();
		const double& inertia() const;
		shape_group& shape();
		const shape_group& shape() const;

		virtual void update(const double& dt);
		vl::vec2d global(const vl::vec2d& local);
		vl::vec2d local (const vl::vec2d& global);
		void apply_force(
			const vl::vec2d& force, 
			const vl::vec2d& point, 
			const double& dt);

		/* Computes and sets inertia value using object mass and area of shape group */
		void adjust_inertia();
		void set_mass(const double& mass, bool adjust_inertia = true);
		void set_density(const double& den, bool adjust_inertia = true);

	private:
		void init();

		vl::vec2d _pos;
		vl::vec2d _vel;
		double _inertia;
		double _rot;
		double _rot_vel;
		double _mass;
		shape_group _shape;
	};
}
