#pragma once
#include "vec2.h"
#include <vector>

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
		double& inertia();
		const double& inertia() const;

		virtual void update(const double& dt);
		vl::vec2d global(const vl::vec2d& local);
		vl::vec2d local (const vl::vec2d& global);
		void apply_force(const vl::vec2d& force, const vl::vec2d& point, const double& dt);

	private:
		void init();

		vl::vec2d _pos;
		vl::vec2d _vel;

		double _inertia;
		double _rot;
		double _rot_vel;
		double _mass;
	};


	// ----------------------------- SHAPE
	class shape
	{
	public:
		shape(const vl::vec2d& loc_pos);
		virtual ~shape() = 0;
		const vl::vec2d& loc_pos() const;
		vl::vec2d& loc_pos();

	protected:
		vl::vec2d _loc_pos;
	};


	// ----------------------------- CIRCLE SHAPE
	class circle_shape : public shape
	{
	public:
		circle_shape(const vl::vec2d& loc_pos);
		const double& radius() const;
		double& radius();

	protected:
		double _radius;
	};


	// ---------------------------- CONVEX SHAPE
	class convex_shape : public shape
	{
	public:
		convex_shape(const vl::vec2d& loc_pos);
		void add(const vl::vec2d& vertex);
		void clear();
		const std::vector<vl::vec2d>& vertices() const;

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
}
