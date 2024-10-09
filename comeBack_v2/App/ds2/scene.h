#pragma once

#include "object.h"
#include "collision_detection.h"
#include "joint.h"

#include <vector>
#include <list>
#include <SFML/Graphics.hpp>

namespace ds2 
{
	class scene
	{
	public:
		scene();
		void add_object(object* object);
		void add_joint(joint* joint);
		void remove(const object* object);
		void remove(const joint* joint);

		void update(const double& dt, sf::RenderWindow& win);

		void set_joint_iterations(const int& joint_iterations);
		const int joint_iterations() const;

		const std::list<object_collision_data>& collisions() const;

	private:
		std::list<object_collision_data> _collisions;
		std::vector<object*> _objects;
		std::list<joint*> _joints;

		int _joint_iterations;

		inline bool overlaping_layers(
			const object* a,
			const object* b);
	};
}


