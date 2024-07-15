#include "scene.h"

void ds2::scene::add_object(const std::shared_ptr<circle_object>& circle){
	_circles.push_back(circle);
}

void ds2::scene::update(const double& dt){
	for (auto& i : _circles) {
		i->update(dt);
	}
	
	_collisions.clear();
	for (auto& i : _circles) {
		for (auto& j : _circles) {
			collision_data cd = collision_detection::check(i, j);
			if (cd.collides) {
				_collisions.push_back(cd);
			}
		}
	}
}

const std::list<ds2::collision_data>& ds2::scene::collisions() const
{
	return _collisions;
}


