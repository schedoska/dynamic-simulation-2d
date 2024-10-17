#pragma once

#include "object.h"

namespace ds2
{
	enum class joint_type {
		joint, spring, hinge, motor
	};

	class joint
	{
	public:
		joint(
			object* a,
			object* b,
			vl::vec2d loc_a = vl::vec2d(),
			vl::vec2d loc_b = vl::vec2d());

		const vl::vec2d& loc_a() const;
		void set_loc_a(const vl::vec2d& loc_a);
		const vl::vec2d& loc_b() const;
		void set_loc_b(const vl::vec2d& loc_b);

		const vl::vec2d global_a() const;
		const vl::vec2d global_b() const;

		object* obj_a();
		object* obj_b();
		void set_obj_a(object* obj_a);
		void set_obj_b(object* obj_b);

		/* dt - delta time, n - amount of iterations per step */
		virtual void update(const double& dt, const unsigned& n = 1);
		virtual joint_type type() const;
		virtual bool iterative() const = 0;

	protected:
		object* _obj_a;
		object* _obj_b;
		vl::vec2d _loc_a;
		vl::vec2d _loc_b;
	};

	class spring_joint : public joint
	{
	public: 
		spring_joint(
			object* a,
			object* b,
			vl::vec2d loc_a = vl::vec2d(),
			vl::vec2d loc_b = vl::vec2d());

		spring_joint(
			object* a,
			object* b,
			const double& strength, 
			const double& damping,
			vl::vec2d loc_a = vl::vec2d(),
			vl::vec2d loc_b = vl::vec2d());

		void update(const double& dt, const unsigned& n = 1) override;
		joint_type type() const override;
		bool iterative() const override;

		const double& length() const;
		void set_length(const double& length);
		const double& strength() const;
		void set_strength(const double& strength);
		const double& damping() const;
		void set_damping(const double& damping);

	protected:
		double _length;
		double _strength;
		double _damping;
	};

	class hinge_joint : public joint
	{
	public:
		hinge_joint(
			object* a,
			object* b,
			vl::vec2d loc_a = vl::vec2d(),
			vl::vec2d loc_b = vl::vec2d(),
			const double& stiffness = 0.1);

		void update(const double& dt, const unsigned& n = 1) override;
		joint_type type() const override;
		bool iterative() const override;

		const double& stiffness() const;
		void set_stiffness(const double& stiffness);
		const double& friction() const;
		void set_friction(const double& friction);
			
	private:
		double _stiffness;
		double _friction;
	};

	class motor_joint : public hinge_joint
	{
	public:	
		motor_joint(
			object* a,
			object* b,
			vl::vec2d loc_a = vl::vec2d(),
			vl::vec2d loc_b = vl::vec2d(),
			const double& beta = 0.1);

		joint_type type() const override;
		void update(const double& dt, const unsigned& n = 1) override;
		bool iterative() const override;

		void set_ang_vel(const double& ang_vel);
		const double& ang_vel() const;
		void set_torque(const double& torque);
		const double& torque() const;
		
	private:
		double _ang_vel;
		double _torque;
	};
}

