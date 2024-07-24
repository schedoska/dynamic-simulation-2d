#pragma once
#include "vec2.h"
#include <vector>

namespace ds2
{
	// ----------------------------- CIRCLE SHAPE
	class circle_shape
	{
	public:
		circle_shape(
			const vl::vec2d& loc_pos = vl::vec2d(), 
			const double& radius = 1.f);
		const double& radius() const;
		double& radius();
		const vl::vec2d& loc_pos() const;
		vl::vec2d& loc_pos();

	protected:
		double _radius;
		vl::vec2d _loc_pos;
	};


	// ---------------------------- CONVEX SHAPE
	class convex_shape
	{
	public:
		convex_shape();
		void add(const vl::vec2d& vertex);
		void clear();
		const std::vector<vl::vec2d>& vertices() const;
		void translate(const vl::vec2d& v);

	protected:
		std::vector<vl::vec2d> _vertices;
	};


	// ---------------------------- SHAPE GROUP
	class shape_group
	{
	public:
		void add(const circle_shape& circle);
		void add(const convex_shape& convex);
		const std::vector<circle_shape>& circles() const;
		const std::vector<convex_shape>& convexes() const;
		void clear();

	private:
		std::vector<circle_shape> _circles;
		std::vector<convex_shape> _convexes;
	};

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
