#include "joint.h"
#include "Utils.h"

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

vl::vec2d& ds2::joint::loc_a()
{
	return _loc_a;
}

const vl::vec2d& ds2::joint::loc_b() const
{
	return _loc_b;
}

vl::vec2d& ds2::joint::loc_b()
{
	return _loc_b;
}

ds2::object* ds2::joint::obj_a()
{
	return _obj_a;
}

ds2::object* ds2::joint::obj_b()
{
	return _obj_b;
}

void ds2::joint::update(const double& dt)
{
	
}

inline double ds2::joint::loc_distance() const
{
	return (_obj_a->global(_loc_a) - _obj_b->global(_loc_b)).len();
}

/*
void ds2::spring_joint::update(const double& dt)
{
	std::shared_ptr<object> a = obj_a().lock();
	std::shared_ptr<object> b = obj_b().lock();

	vl::vec2d dv = a->global(_loc_a) - b->global(_loc_b);
	double str = _len - dv.len();
	dv.normalize();

	if (_fixed_a) {
		double angle_diff = _rot_a - a->rot();
		a->rot_vel() += (angle_diff * 75000000 - a->rot_vel() * 3000000) / a->inertia() * dt;
	}
	if (_fixed_b) {
		double angle_diff = _rot_b - b->rot();
		b->rot_vel() += (angle_diff * 75000000 - b->rot_vel() * 3000000) / a->inertia() * dt;
	}

	vl::vec2d delta_vel = dv * (a->vel() - b->vel()).dot(dv);
	a->apply_force(dv * str * _stiff - delta_vel * _damp, _loc_a, dt);
	b->apply_force(dv * -str * _stiff + delta_vel * _damp, _loc_b, dt);
}

ds2::fixed_joint::fixed_joint(
	const std::shared_ptr<object>& a, 
	const std::shared_ptr<object>& b, 
	vl::vec2d loc_a, 
	vl::vec2d loc_b)
	: spring_joint(a, b, loc_a, loc_b)
{
	_damp = 1000;
	_stiff = 20000;
	_len = 0.0;
}
*/

ds2::spring::spring(object* a, object* b, vl::vec2d loc_a, vl::vec2d loc_b)
	: joint(a, b, loc_a, loc_b)
{
	_length = loc_distance();
	brace();
}

ds2::spring::spring(object* a, object* b, const double& strength, const double& damping, vl::vec2d loc_a, vl::vec2d loc_b)
	: joint(a, b, loc_a, loc_b), _strength(strength), _damping(damping)
{
	_length = loc_distance();
}

void ds2::spring::update(const double& dt)
{
	vl::vec2d dv = _obj_a->global(_loc_a) - _obj_b->global(_loc_b);
	double dist = dv.len();
	dv.normalize();
	dv *= _strength * (dist - _length) + _damping * (_obj_a->vel() - _obj_b->vel()).dot(dv);

	_obj_a->apply_force(dv * -1.0, _loc_a, dt);
	_obj_b->apply_force(dv, _loc_b, dt);
}

void ds2::spring::brace()
{
	//_strength = 27000;
	//_damping = 220;
	_strength = 7700;
	_damping = 200;
}

const double& ds2::spring::length() const
{
	return _length;
}

double& ds2::spring::length()
{
	return _length;
}

const double& ds2::spring::strength() const
{
	return _strength;
}

double& ds2::spring::strength()
{
	return _strength;
}

const double& ds2::spring::damping() const
{
	return _damping;
}

double& ds2::spring::damping()
{
	return _damping;
}

ds2::hinge_joint::hinge_joint(
	object* a, 
	object* b, 
	vl::vec2d loc_a, 
	vl::vec2d loc_b,
	const double& beta)
	: joint(a, b, loc_a, loc_b), _beta(beta)
{
}

/*
void ds2::hinge_joint::update(const double& dt)
{
	double tm = _obj_a->mass() + _obj_b->mass();
	vl::vec2d cpos =
		_obj_a->global(_loc_a) * _obj_a->mass() +
		_obj_b->global(_loc_b) * _obj_b->mass();
	cpos /= tm;

	vl::vec2d a1 = _obj_a->global(_loc_a) - _obj_a->pos();
	vl::vec2d a2 = cpos - _obj_a->pos();
	double a_dang = utils::angle2(a1, a2) * _beta;
	_obj_a->rot() += a_dang;
	_obj_a->rot_vel() += (a_dang / dt) * 1;

	vl::vec2d b1 = _obj_b->global(_loc_b) - _obj_b->pos();
	vl::vec2d b2 = cpos - _obj_b->pos();
	double b_dang = utils::angle2(b1, b2) * _beta;
	_obj_b->rot() += b_dang;
	_obj_b->rot_vel() += (b_dang / dt) * 1;

	vl::vec2d a_dpos = (cpos - _obj_a->global(_loc_a)) * _beta;
	_obj_a->pos() += a_dpos;
	_obj_a->vel() += (a_dpos / dt) * 1;

	vl::vec2d b_dpos = (cpos - _obj_b->global(_loc_b)) * _beta;
	_obj_b->pos() += b_dpos;
	_obj_b->vel() += (b_dpos / dt) * 1;
}
*/

void ds2::hinge_joint::update(const double& dt)
{
	double tm = _obj_a->mass() + _obj_b->mass();
	vl::vec2d cpos =
		_obj_a->global(_loc_a) * _obj_a->mass() +
		_obj_b->global(_loc_b) * _obj_b->mass();
	cpos /= tm;

	vl::vec2d a1 = _obj_a->global(_loc_a) - _obj_a->pos();
	vl::vec2d a2 = cpos - _obj_a->pos();
	double a_dang = utils::angle2(a1, a2) * _beta;
	_obj_a->rot() += a_dang;
	_obj_a->rot_vel() += (a_dang / dt) * 1;

	vl::vec2d b1 = _obj_b->global(_loc_b) - _obj_b->pos();
	vl::vec2d b2 = cpos - _obj_b->pos();
	double b_dang = utils::angle2(b1, b2) * _beta;
	_obj_b->rot() += b_dang;
	_obj_b->rot_vel() += (b_dang / dt) * 1;

	vl::vec2d a_dpos = (cpos - _obj_a->global(_loc_a)) * _beta;
	_obj_a->pos() += a_dpos;
	_obj_a->vel() += (a_dpos / dt) * 1;

	vl::vec2d b_dpos = (cpos - _obj_b->global(_loc_b)) * _beta;
	_obj_b->pos() += b_dpos;
	_obj_b->vel() += (b_dpos / dt) * 1;
}

const double& ds2::hinge_joint::beta() const
{
	return _beta;
}

double& ds2::hinge_joint::beta()
{
	return _beta;
}

ds2::motor_joint::motor_joint(
	object* a,
	object* b,
	const double& speed,
	vl::vec2d loc_a,
	vl::vec2d loc_b,
	const double& beta)
	: hinge_joint(a, b, loc_a, loc_b, beta), _speed(speed)
{
	_torque = 300;
}

void ds2::motor_joint::update(const double& dt)
{
	hinge_joint::update(dt);

	vl::vec2d d = _obj_b->pos() - _obj_b->global(_loc_b);
	d.normalize();
	vl::vec2d perp = { -d[1], d[0] };

	if (std::abs(perp.dot(_obj_b->vel())) < _speed)
	{
		_obj_b->apply_force_local({ 0, _torque }, _loc_b + vl::vec2d(100, 0), dt);
		_obj_b->apply_force_local({ 0, -_torque }, _loc_b - vl::vec2d(100, 0), dt);
	}
	else {
		_obj_b->apply_force_local({ 0, -_torque }, _loc_b + vl::vec2d(100, 0), dt);
		_obj_b->apply_force_local({ 0, _torque }, _loc_b - vl::vec2d(100, 0), dt);
	}
}
