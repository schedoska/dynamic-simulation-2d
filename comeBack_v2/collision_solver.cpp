#include "collision_solver.h"
#include "Utils.h"

void ds2::collision_solver::solve_collision(const collision_data& cd, sf::RenderWindow& win)
{
	std::shared_ptr<object> obj_a = cd.a.lock();
	std::shared_ptr<object> obj_b = cd.b.lock();

	/* Moving two object away from eachother */
	vl::vec2d dv = cd.cp_a - cd.cp_b;
	double mass_total = obj_a->mass() + obj_b->mass();

	obj_a->pos() += dv * -obj_b->mass() / mass_total;
	obj_b->pos() += dv * obj_a->mass() / mass_total;

	/* Compute new object speed */
	vl::vec2d mass_cp_a = cd.cp_a - obj_a->pos();
	vl::vec2d mass_cp_a_norm = mass_cp_a;
	mass_cp_a_norm.normalize();

	vl::vec2d mass_cp_b = cd.cp_b - obj_b->pos();
	vl::vec2d mass_cp_b_norm = mass_cp_b;
	mass_cp_b_norm.normalize();

	/* DEBUG DRAW */
	utils::drawLine(obj_a->pos(), obj_a->pos() + mass_cp_a, win, sf::Color::Green);
	utils::drawLine(obj_b->pos(), obj_b->pos() + mass_cp_b, win, sf::Color::Green);
	/* DEBUG DRAW */



	// ------------------------------ TEST CODE REMOVE ARTERWARDS --- BEGININING
	vl::vec2d mass_cp_a_perp = vl::vec2d(-mass_cp_a_norm[1], mass_cp_a_norm[0]);
	vl::vec2d mass_cp_b_perp = vl::vec2d(-mass_cp_b_norm[1], mass_cp_b_norm[0]);

	

	return;
	// ------------------------------ TEST CODE REMOVE ARTERWARDS --- END







	#ifdef TEST

	vl::vec2d mass_cp_a_perp = vl::vec2d(-mass_cp_a_norm[1], mass_cp_a_norm[0]);
	vl::vec2d mass_cp_b_perp = vl::vec2d(-mass_cp_b_norm[1], mass_cp_b_norm[0]);
	
	vl::vec2d vel_cp_a = obj_a->vel() + mass_cp_a_perp * obj_a->rot_vel() * mass_cp_a.len();
	vl::vec2d vel_cp_b = obj_b->vel() + mass_cp_b_perp * obj_b->rot_vel() * mass_cp_b.len();

	vl::vec2d new_vel_cp_a = vel_cp_a * (obj_a->mass() - obj_b->mass()) / mass_total +
		vel_cp_b * 2 * obj_b->mass() / mass_total;

	std::cout << vel_cp_a << "   " << new_vel_cp_a << "\n";

	vl::vec2d new_vel_cp_b = vel_cp_a * 2 * obj_a->mass() / mass_total +
		vel_cp_b * (obj_b->mass() - obj_a->mass()) / mass_total;

	/* DEBUG DRAW */
	utils::drawLine(cd.cp_a, cd.cp_a + new_vel_cp_a * 3, win, sf::Color::Magenta);
	utils::drawLine(cd.cp_b, cd.cp_b + new_vel_cp_b * 3, win, sf::Color::Magenta);
	/* DEBUG DRAW */

	/* Compute new tranlation and angular velocities allowing for contact point movements */
	obj_b->vel() = mass_cp_b_norm * new_vel_cp_b.dot(mass_cp_b_norm) * 0.6;
	obj_b->rot_vel() = new_vel_cp_b.dot(mass_cp_b_perp) / mass_cp_b.len() * 0.6;
	
	obj_a->vel() = mass_cp_a_norm * new_vel_cp_a.dot(mass_cp_a_norm) * 0.6;
	obj_a->rot_vel() = new_vel_cp_a.dot(mass_cp_a_perp) / mass_cp_a.len() * 0.6;

	#endif
}
