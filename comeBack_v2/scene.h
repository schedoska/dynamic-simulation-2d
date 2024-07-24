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
		void add_object(const std::shared_ptr<object>& object);
		void add_joint(joint* j);
		void update(const double& dt, sf::RenderWindow& win);

		const std::list<object_collision_data>& collisions() const;

	private:
		std::list<object_collision_data> _collisions;
		std::vector<std::shared_ptr<object>> _objects;
		std::list<joint*> _joints;
	};
}


