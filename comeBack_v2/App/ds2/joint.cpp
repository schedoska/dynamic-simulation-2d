#include "joint.h"
#include "Utils.h"

#define _USE_MATH_DEFINES
#include <math.h>

ds2::joint::joint(
	object* obj_a,
	object* obj_b,
	vl::vec2d loc_a, 
	vl::vec2d loc_b)
	: _obj_a(obj_a), _obj_b(obj_b), _loc_a(loc_a), _loc_b(loc_b)
{}

const vl::vec2d& ds2::joint::loc_a() const
{
	return _loc_a;
}

void ds2::joint::set_loc_a(const vl::vec2d& loc_a)
{
	_loc_a = loc_a;
}

const vl::vec2d& ds2::joint::loc_b() const
{
	return _loc_b;
}

void ds2::joint::set_loc_b(const vl::vec2d& loc_b)
{
	_loc_b = loc_b;
}

const vl::vec2d ds2::joint::global_a() const
{
	if (!_obj_a) {
		return _loc_a;
	}
	else {
		return _obj_a->global(_loc_a);
	} 
}

const vl::vec2d ds2::joint::global_b() const
{
	if (!_obj_b) {
		return _loc_b;
	}
	else {
		return _obj_b->global(_loc_b);
	}
}

ds2::object* ds2::joint::obj_a()
{
	return _obj_a;
}

ds2::object* ds2::joint::obj_b()
{
	return _obj_b;
}

void ds2::joint::set_obj_a(object* obj_a)
{
	_obj_a = obj_a;
}

void ds2::joint::set_obj_b(object* obj_b)
{
	_obj_b = obj_b;
}

void ds2::joint::update(const double& dt, const unsigned& n)
{
	
}

ds2::joint_type ds2::joint::type() const
{
	return joint_type::joint;
}

ds2::spring_joint::spring_joint(object* a, object* b, vl::vec2d loc_a, vl::vec2d loc_b)
	: joint(a, b, loc_a, loc_b)
{
	_length = 200;
	_strength = 30;
	_damping = 20;
}

ds2::spring_joint::spring_joint(object* a, object* b, const double& strength, const double& damping, vl::vec2d loc_a, vl::vec2d loc_b)
	: joint(a, b, loc_a, loc_b), _strength(strength), _damping(damping)
{
}

void ds2::spring_joint::update(const double& dt, const unsigned& n)
{
	vl::vec2d dv = _obj_a->global(_loc_a) - _obj_b->global(_loc_b);
	double dist = dv.len();
	dv.normalize();
	double rel_vel = (_obj_a->vel() - _obj_b->vel()).dot(dv);
	vl::vec2d f = dv * (_strength * (dist - _length) + _damping * rel_vel);

	_obj_a->apply_force(f * -1.0, _loc_a, dt);
	_obj_b->apply_force(f, _loc_b, dt);
}

ds2::joint_type ds2::spring_joint::type() const
{
	return joint_type::spring;
}

bool ds2::spring_joint::iterative() const
{
	return false;
}

const double& ds2::spring_joint::length() const
{
	return _length;
}

void ds2::spring_joint::set_length(const double& length)
{
	_length = length;
}

const double& ds2::spring_joint::strength() const
{
	return _strength;
}

void ds2::spring_joint::set_strength(const double& strength)
{
	_strength = strength;
}

const double& ds2::spring_joint::damping() const
{
	return _damping;
}

void ds2::spring_joint::set_damping(const double& damping)
{
	_damping = damping;
}

double ds2::spring_joint::loc_distance() const
{
	return (global_a() - global_b()).len();
}

ds2::hinge_joint::hinge_joint(
	object* a, 
	object* b, 
	vl::vec2d loc_a, 
	vl::vec2d loc_b,
	const double& stiffness)
	: joint(a, b, loc_a, loc_b), _stiffness(stiffness)
{
	_friction = 0.8;
}

void ds2::hinge_joint::update(const double& dt, const unsigned& n)
{
	double tm = _obj_a->mass() + _obj_b->mass();
	vl::vec2d cpos =
		_obj_a->global(_loc_a) * _obj_a->mass() +
		_obj_b->global(_loc_b) * _obj_b->mass();
	cpos /= tm;

	vl::vec2d a1 = _obj_a->global(_loc_a) - _obj_a->pos();
	vl::vec2d a2 = cpos - _obj_a->pos();
	double a_dang = utils::angle2(a1, a2) * _stiffness;
	_obj_a->rot() += a_dang;
	_obj_a->rot_vel() += (a_dang / dt) * _friction;

	vl::vec2d b1 = _obj_b->global(_loc_b) - _obj_b->pos();
	vl::vec2d b2 = cpos - _obj_b->pos();
	double b_dang = utils::angle2(b1, b2) * _stiffness;
	_obj_b->rot() += b_dang;
	_obj_b->rot_vel() += (b_dang / dt) * _friction;

	vl::vec2d a_dpos = (cpos - _obj_a->global(_loc_a)) * _stiffness;
	_obj_a->pos() += a_dpos;
	_obj_a->vel() += (a_dpos / dt) * _friction;

	vl::vec2d b_dpos = (cpos - _obj_b->global(_loc_b)) * _stiffness;
	_obj_b->pos() += b_dpos;
	_obj_b->vel() += (b_dpos / dt) * _friction;
}

ds2::joint_type ds2::hinge_joint::type() const
{
	return joint_type::hinge;
}

bool ds2::hinge_joint::iterative() const
{
	return true;
}

const double& ds2::hinge_joint::stiffness() const
{
	return _stiffness;
}

void ds2::hinge_joint::set_stiffness(const double& stiffness)
{
	_stiffness = stiffness;
}

const double& ds2::hinge_joint::friction() const
{
	return _friction;
}

void ds2::hinge_joint::set_friction(const double& friction)
{
	_friction = friction;
}

ds2::motor_joint::motor_joint(
	object* a,
	object* b,
	vl::vec2d loc_a,
	vl::vec2d loc_b,
	const double& beta)
	: hinge_joint(a, b, loc_a, loc_b, beta)
{
	_torque = 0;
	_ang_vel = 0;
	_last_rel_rot = 0;
	_it_counter = 0;
}

ds2::joint_type ds2::motor_joint::type() const
{
	return joint_type::motor;
}

void ds2::motor_joint::update(const double& dt, const unsigned& n)
{
	hinge_joint::update(dt);

	vl::vec2d b_loc_v = _obj_b->pos() - _obj_b->global(_loc_b);
	vl::vec2d a_loc_v = _obj_a->pos() - _obj_a->global(_loc_a);

	if (++_it_counter == n) {
		_it_counter = 0;
		double prev_vel = _rel_rot_vel;
		_rel_rot = utils::angle2(b_loc_v, a_loc_v);
		_rel_rot_vel = (_rel_rot - _last_rel_rot);

		double d_v = _rel_rot_vel - prev_vel;

		if (_rel_rot < -M_PI_2 && _last_rel_rot > M_PI_2) _rel_rot_vel += 2.0 * M_PI;
		if (_rel_rot > M_PI_2 && _last_rel_rot < -M_PI_2) _rel_rot_vel -= 2.0 * M_PI;
		_last_rel_rot = _rel_rot;
		
		double force = _torque * (_rel_rot_vel / dt - _ang_vel);
		force = std::min(std::max(force, -100.0), 100.0);

		_obj_b->apply_force_local({ 0, force }, _loc_b + vl::vec2d(100, 0), dt);
		_obj_b->apply_force_local({ 0, -force }, _loc_b - vl::vec2d(100, 0), dt);
		_obj_a->apply_force_local({ 0, -force }, _loc_a + vl::vec2d(100, 0), dt);
		_obj_a->apply_force_local({ 0, force }, _loc_a - vl::vec2d(100, 0), dt);
	}
}

bool ds2::motor_joint::iterative() const
{
	return true;
}

void ds2::motor_joint::set_ang_vel(const double& ang_vel)
{
	_ang_vel = ang_vel;
}

const double& ds2::motor_joint::ang_vel() const
{
	return _ang_vel;
}

void ds2::motor_joint::set_torque(const double& torque)
{
	_torque = torque;
}

const double& ds2::motor_joint::torque() const
{
	return _torque;
}

const double& ds2::motor_joint::rel_rot() const
{
	return _rel_rot;
}

const double& ds2::motor_joint::rel_rot_vel() const
{
	return _rel_rot_vel;
}
