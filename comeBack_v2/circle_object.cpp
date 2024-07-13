#include "circle_object.h"

ds2::circle_object::circle_object(const vl::vec2d& pos, const double& radius)
	: object(pos), _radius(radius) {}

double& ds2::circle_object::radius(){
	return _radius;
}

const double& ds2::circle_object::radius() const{
	return _radius;
}
