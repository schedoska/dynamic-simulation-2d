#pragma once

#include "circle_object.h"
#include "convex_object.h"
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
		void add_object(const std::shared_ptr<circle_object>& circle);
		void add_object(const std::shared_ptr<convex_object>& convex);
		void add_joint(joint* j);
		void update(const double& dt, sf::RenderWindow& win);

		const std::list<collision_data>& collisions() const;

	private:
		std::list<collision_data> _collisions;
		std::vector<std::shared_ptr<circle_object>> _circle_shapes;
		std::vector<std::shared_ptr<convex_object>> _convex_shapes;

		std::list<joint*> _joints;
	};
}


