#include "scene.h"
#include "collision_solver.h"

ds2::scene::scene()
{
    _joint_iterations = 3;
}

void ds2::scene::add(object* object)
{
    _objects.push_back(object);
}

void ds2::scene::add(joint* j)
{
    _joints.push_back(j);
}

void ds2::scene::remove(const object* object)
{
    _objects.erase(std::remove(_objects.begin(), _objects.end(), object));
}

void ds2::scene::remove(const joint* joint)
{
    _joints.erase(std::remove(_joints.begin(), _joints.end(), joint));
}

void ds2::scene::remove_all()
{
    _objects.clear();
    _joints.clear();
}

const std::vector<ds2::object*>& ds2::scene::objects() const
{
    return _objects;
}

const std::vector<ds2::joint*>& ds2::scene::joints() const
{
    return _joints;
}


void ds2::scene::update(const double& dt, sf::RenderWindow& win){
    for (auto& i : _objects)
        i->update(dt);

    for (int j = 0; j < _joint_iterations; ++j) {
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

void ds2::scene::set_joint_iterations(const int& joint_iterations)
{
    _joint_iterations = joint_iterations;
}

const int ds2::scene::joint_iterations() const
{
    return _joint_iterations;
}

const std::list<ds2::object_collision_data>& ds2::scene::collisions() const
{
	return _collisions;
}

inline bool ds2::scene::overlaping_layers(const object* a, const object* b)
{
    return a->layer_min() <= b->layer_max() && b->layer_min() <= a->layer_max();
}


