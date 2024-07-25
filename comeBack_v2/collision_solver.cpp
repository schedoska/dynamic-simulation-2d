#include "collision_solver.h"
#include "Utils.h"
#include <chrono>
#include <thread>

void ds2::collision_solver::solve_collision(const object_collision_data& cd, sf::RenderWindow& win)
{
	std::shared_ptr<object> obj_a = cd.a.lock(); 
	std::shared_ptr<object> obj_b = cd.b.lock();

	/* Moving two object away from eachother */
	vl::vec2d dv = cd.data.cp_a - cd.data.cp_b;
	double mass_total = obj_a->mass() + obj_b->mass();

	const double seperation = 1.4;
	obj_a->pos() += dv * -obj_b->mass() * seperation / mass_total;
	obj_b->pos() += dv * obj_a->mass() * seperation / mass_total;

	/* Compute new object speed */
	vl::vec2d mass_cp_a = cd.data.cp_a - obj_a->pos();
	vl::vec2d mass_cp_a_norm = mass_cp_a;
	mass_cp_a_norm.normalize();

	vl::vec2d mass_cp_b = cd.data.cp_b - obj_b->pos();
	vl::vec2d mass_cp_b_norm = mass_cp_b;
	mass_cp_b_norm.normalize();

	/* DEBUG DRAW */
	utils::drawLine(obj_a->pos(), obj_a->pos() + mass_cp_a, win, sf::Color::Green);
	utils::drawLine(obj_b->pos(), obj_b->pos() + mass_cp_b, win, sf::Color::Green);

	dv.normalize();

	vl::vec2d mass_cp_a_perp = vl::vec2d(-mass_cp_a_norm[1], mass_cp_a_norm[0]);
	vl::vec2d mass_cp_b_perp = vl::vec2d(-mass_cp_b_norm[1], mass_cp_b_norm[0]);

	vl::vec2d vel_cp_a = obj_a->vel() + mass_cp_a_perp * obj_a->rot_vel() * mass_cp_a.len();
	vl::vec2d vel_cp_b = obj_b->vel() + mass_cp_b_perp * obj_b->rot_vel() * mass_cp_b.len();
	vl::vec2d vr = vel_cp_b - vel_cp_a;
	double vj = 1.4 * vr.dot(dv);

	double ma_inv = 1.0 / obj_a->mass();
	double mb_inv = 1.0 / obj_b->mass();

	double J = vj / (ma_inv + mb_inv + std::pow(utils::cross(mass_cp_a, dv), 2) / obj_a->inertia() +
		std::pow(utils::cross(mass_cp_b, dv), 2) / obj_b->inertia());

	obj_a->vel() += dv * ma_inv * J;
	obj_b->vel() -= dv * mb_inv * J;
	obj_a->rot_vel() += utils::cross(mass_cp_a, dv * J) / obj_a->inertia();
	obj_b->rot_vel() -= utils::cross(mass_cp_b, dv * J) / obj_b->inertia();

	
	/* Friction impulses */
	vl::vec2d dv_perp = vl::vec2d(dv[1], -dv[0]);
	double fj = 0.1 * vr.dot(dv_perp);

	double Jf = fj / (ma_inv + mb_inv + std::pow(vl::cross(mass_cp_a, dv_perp), 2) / obj_a->inertia() +
		std::pow(vl::cross(mass_cp_b, dv_perp), 2) / obj_b->inertia());

	obj_a->vel() += dv_perp * ma_inv * Jf;
	obj_b->vel() -= dv_perp * mb_inv * Jf;
	obj_a->rot_vel() += vl::cross(mass_cp_a, dv_perp * Jf) / obj_a->inertia();
	obj_b->rot_vel() -= vl::cross(mass_cp_b, dv_perp * Jf) / obj_b->inertia();

	/* Fixes Flickering */
	if (std::abs(obj_a->vel()[0]) < 5) obj_a->vel()[0] = 0;
	if (std::abs(obj_a->vel()[1]) < 5) obj_a->vel()[1] = 0;
	if (std::abs(obj_b->vel()[0]) < 5) obj_b->vel()[0] = 0;
	if (std::abs(obj_b->vel()[1]) < 5) obj_b->vel()[1] = 0;
}
