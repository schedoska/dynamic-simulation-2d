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

	class fixed_joint : public joint
	{
	public:
		fixed_joint(
			object* a, 
			object* b,
			vl::vec2d loc_a = vl::vec2d(),
			vl::vec2d loc_b = vl::vec2d());
		void update(const double& dt) override;
	};
}
