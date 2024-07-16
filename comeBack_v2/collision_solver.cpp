#include "collision_solver.h"

void ds2::collision_solver::solve_collision(const collision_data& cd)
{
	std::shared_ptr<object> obj_a = cd.a.lock();
	std::shared_ptr<object> obj_b = cd.b.lock();

	/* Moving two object away from eachother */
	vl::vec2d dv = cd.cp_a - cd.cp_b;
	double mass_total = obj_a->mass() + obj_b->mass();

	obj_a->pos() += dv * -obj_a->mass() / mass_total;
	obj_b->pos() += dv * obj_b->mass() / mass_total;

	/* Compute new object speed */
	vl::vec2d mass_cp_a = obj_a->pos() - cd.cp_a;
	vl::vec2d mass_cp_a_norm = mass_cp_a;
	mass_cp_a_norm.normalize();

	vl::vec2d coll_vec = cd.cp_b - cd.cp_a;
	vl::vec2d coll_vec_norm = coll_vec;
	coll_vec_norm.normalize();

	double val = coll_vec_norm.dot(mass_cp_a_norm);


	vl::vec2d perp_mass_cp_a = vl::vec2d(mass_cp_a_norm[1], -mass_cp_a_norm[0]);

	double val2 = coll_vec_norm.dot(perp_mass_cp_a);

	//

	obj_a->vel() += 

	
}
