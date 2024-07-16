#include "scene.h"

void ds2::scene::add_object(const std::shared_ptr<circle_object>& circle){
	_circle_shapes.push_back(circle);
}

void ds2::scene::add_object(const std::shared_ptr<convex_object>& convex) {
	_convex_shapes.push_back(convex);
}

void ds2::scene::update(const double& dt){
	for (auto& i : _circle_shapes) {
		i->update(dt);
	}
	for (auto& i : _convex_shapes) {
		i->update(dt);
	}
	
	_collisions.clear();
	for (auto& i : _circle_shapes) {
		for (auto& j : _circle_shapes) {
			collision_data cd = collision_detection::check(i, j);
			if (cd.collides) {
				_collisions.push_back(cd);
			}
		}
	}

	_collisions.clear();
	for (auto& i : _circle_shapes) {
		for (auto& j : _convex_shapes) {
			collision_data cd = collision_detection::check(j, i);
			if (cd.collides) {
				_collisions.push_back(cd);
			}
		}
	}

	int c = 0;
	for (auto& i : _convex_shapes) {
		for (auto& j : _convex_shapes) {
			//if (c >= 2) continue;
			collision_data cd = collision_detection::check(i, j);
			if (cd.collides) {
				_collisions.push_back(cd);
			}
			c++;
		}
	}
}

const std::list<ds2::collision_data>& ds2::scene::collisions() const
{
	return _collisions;
}


