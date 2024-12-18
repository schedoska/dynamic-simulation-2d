#pragma once

#include "object.h"
#include "collision_detection.h"
#include "joint.h"

#include <vector>
#include <list>
#include <SFML/Graphics.hpp>
#include <functional>

namespace ds2 
{
	class scene
	{
	public:
		scene();
		void add(object* object);
		void add(joint* joint);
		void remove(const object* object);
		void remove(const joint* joint);
		// removes all objects and joints
		void remove_all();

		const std::vector<object*>& objects() const;
		const std::vector<joint*>& joints() const;
		const std::vector<joint*>& iterative_joints() const;

		void update(const double& dt, sf::RenderWindow& win);

		void set_joint_iterations(const int& joint_iterations);
		const int joint_iterations() const;
		void set_gravity_v(const vl::vec2d& gravity_vec);
		const vl::vec2d& gravity_v() const;

		const std::list<object_collision_data>& collisions() const;

		void apply_to_objects(std::function<void(object*)> func);

	private:
		std::list<object_collision_data> _collisions;
		std::vector<object*> _objects;
		std::vector<joint*> _joints;
		std::vector<joint*> _iterative_joints;

		int _joint_iterations;

		inline bool overlaping_layers(
			const object* a,
			const object* b);

		vl::vec2d _gravity_v;
	};
}


