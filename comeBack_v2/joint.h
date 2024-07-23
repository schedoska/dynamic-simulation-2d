#pragma once

#include "object.h"

namespace ds2
{
	class joint
	{
	public:
		joint(
			const std::shared_ptr<object>& a, 
			const std::shared_ptr<object>& b,
			vl::vec2d loc_a = vl::vec2d(),
			vl::vec2d loc_b = vl::vec2d());

		const vl::vec2d& loc_a() const;
		vl::vec2d& loc_a();
		const vl::vec2d& loc_b() const;
		vl::vec2d& loc_b();

		std::weak_ptr<object> obj_a();
		std::weak_ptr<object> obj_b();

		virtual void update(const double& dt) = 0;

	protected:
		std::weak_ptr<object> _obj_a;
		std::weak_ptr<object> _obj_b;
		vl::vec2d _loc_a;
		vl::vec2d _loc_b;
	};

	class spring_joint : public joint
	{
	public:
		spring_joint(
			const std::shared_ptr<object>& a,
			const std::shared_ptr<object>& b,
			vl::vec2d loc_a = vl::vec2d(),
			vl::vec2d loc_b = vl::vec2d(),
			const bool fixed_a = false,
			const bool fixed_b = false);

		const double& stiff() const;
		double& stiff();
		const double& damp() const;
		double& damp();
		const double& len() const;
		double& len();

		void update(const double& dt) override;

	protected:
		double _stiff;
		double _damp;
		double _len;

		double _rot_a;
		double _rot_b;
		bool _fixed_a;
		bool _fixed_b;
	};

	class fixed_joint : public spring_joint
	{
	public:
		fixed_joint(
			const std::shared_ptr<object>& a,
			const std::shared_ptr<object>& b,
			vl::vec2d loc_a = vl::vec2d(),
			vl::vec2d loc_b = vl::vec2d());

		//void update(const double& dt) override;
	};


}
