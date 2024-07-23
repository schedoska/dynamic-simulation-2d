#include "joint.h"

ds2::joint::joint(
	const std::shared_ptr<ds2::object>& obj_a,
	const std::shared_ptr<ds2::object>& obj_b,
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

std::weak_ptr<ds2::object> ds2::joint::obj_a()
{
	return _obj_a;
}

std::weak_ptr<ds2::object> ds2::joint::obj_b()
{
	return _obj_b;
}

ds2::spring_joint::spring_joint(
	const std::shared_ptr<object>& a,
	const std::shared_ptr<object>& b,
	vl::vec2d loc_a,
	vl::vec2d loc_b)
	: joint(a, b, loc_a, loc_b)
{
	_stiff = 500;
	_damp = 100;
	_len = 100;
}

const double& ds2::spring_joint::stiff() const
{
	return _stiff;
}

double& ds2::spring_joint::stiff()
{
	return _stiff;
}

const double& ds2::spring_joint::damp() const
{
	return _damp;
}

double& ds2::spring_joint::damp()
{
	return _damp;
}

const double& ds2::spring_joint::len() const
{
	return _len;
}

double& ds2::spring_joint::len()
{
	return _len;
}

void ds2::spring_joint::update(const double& dt)
{
	std::shared_ptr<object> a = obj_a().lock();
	std::shared_ptr<object> b = obj_b().lock();

	vl::vec2d dv = a->global(_loc_a) - b->global(_loc_b);
	double str = _len - dv.len();
	dv.normalize();

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


