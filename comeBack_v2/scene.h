#pragma once

#include "circle_object.h"
#include "convex_object.h"
#include "collision_detection.h"


#include <vector>
#include <list>

namespace ds2 
{
	class scene
	{
	public:
		void add_object(const std::shared_ptr<circle_object>& circle);
		void update(const double& dt);

		const std::list<collision_data>& collisions() const;

	private:
		std::list<collision_data> _collisions;
		std::vector<std::shared_ptr<circle_object>> _circles;
	};
}


