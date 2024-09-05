#include "scene.h"
#include "collision_solver.h"

void ds2::scene::add_object(const std::shared_ptr<object>& object)
{
    _objects.push_back(object);
}

void ds2::scene::add_joint(joint* j)
{
    _joints.push_back(j);
}

void ds2::scene::update(const double& dt, sf::RenderWindow& win){
    for (auto& i : _objects)
        i->update(dt);

    for (int j = 0; j < 10; ++j) {
        for (auto i : _joints) {
            i->update(dt);
        }
    }

    for (auto a = _objects.begin(); a != _objects.end(); ++a) {
        for (auto b = a + 1; b != _objects.end(); ++b) {
            if (!overlaping_layers(*a,*b))
                continue;
            object_collision_data ocd = collision_detection::check(*a, *b);
            if (ocd.data.collides == false) 
                continue;
            collision_solver::solve_collision(ocd, win);
        }
    }    

}

const std::list<ds2::object_collision_data>& ds2::scene::collisions() const
{
	return _collisions;
}

inline bool ds2::scene::overlaping_layers(const std::shared_ptr<object>& a, const std::shared_ptr<object>& b)
{
    return a->layer_min() <= b->layer_max() && b->layer_min() <= a->layer_max();
}


