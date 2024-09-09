#pragma once

#include "object.h"

namespace ds2
{
	class joint
	{
	public:
		joint(
			object* a,
			object* b,
			vl::vec2d loc_a = vl::vec2d(),
			vl::vec2d loc_b = vl::vec2d());

		const vl::vec2d& loc_a() const;
		vl::vec2d& loc_a();
		const vl::vec2d& loc_b() const;
		vl::vec2d& loc_b();

		object* obj_a();
		object* obj_b();

		virtual void update(const double& dt);

	protected:
		object* _obj_a;
		object* _obj_b;
		vl::vec2d _loc_a;
		vl::vec2d _loc_b;

		inline double loc_distance() const;
	};

	class spring : public joint
	{
	public: 
		spring(
			object* a,
			object* b,
			vl::vec2d loc_a = vl::vec2d(),
			vl::vec2d loc_b = vl::vec2d());

		spring(
			object* a,
			object* b,
			const double& strength, 
			const double& damping,
			vl::vec2d loc_a = vl::vec2d(),
			vl::vec2d loc_b = vl::vec2d());

		void update(const double& dt) override;
		void brace();

		const double& length() const;
		double& length();
		const double& strength() const;
		double& strength();
		const double& damping() const;
		double& damping();

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
			const double& beta = 0.1);

		void update(const double& dt) override;
		const double& beta() const;
		double& beta();

	private:
		double _beta;
	};

	class motor_joint : public hinge_joint
	{
	public:	
		motor_joint(
			object* a,
			object* b,
			const double& speed,
			vl::vec2d loc_a = vl::vec2d(),
			vl::vec2d loc_b = vl::vec2d(),
			const double& beta = 0.1);

		void update(const double& dt) override;
		
		double _speed;
		double _torque;
	};
}
