#include "scene.h"
#include "collision_solver.h"

ds2::scene::scene()
{
    _joint_iterations = 50;
}

void ds2::scene::add(object* object)
{
    _objects.push_back(object);
}

void ds2::scene::add(joint* j)
{
    j->iterative() ? _iterative_joints.push_back(j) : _joints.push_back(j);
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
    _iterative_joints.clear();
}

const std::vector<ds2::object*>& ds2::scene::objects() const
{
    return _objects;
}

const std::vector<ds2::joint*>& ds2::scene::joints() const
{
    return _joints;
}

const std::vector<ds2::joint*>& ds2::scene::iterative_joints() const
{
    return _iterative_joints;
}

void ds2::scene::update(const double& dt, sf::RenderWindow& win){
    if (dt == 0.0) return;

    for (auto i : _joints) {
        i->update(dt);
    }
    for (auto& i : _objects) {
        if (!i->is_static()) i->vel() += _gravity_v * dt;
        i->update(dt);
    }

    for (int j = 0; j < _joint_iterations; ++j) {
        for (auto i : _iterative_joints) {
            i->update(dt, _joint_iterations);
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

void ds2::scene::set_gravity_v(const vl::vec2d& v)
{
    _gravity_v = v;
}

const vl::vec2d& ds2::scene::gravity_v() const
{
    return _gravity_v;
}

const std::list<ds2::object_collision_data>& ds2::scene::collisions() const
{
	return _collisions;
}

void ds2::scene::apply_to_objects(std::function<void(object*)> func)
{
    assert(func && "Passed function is invalid");
    for (object* obj : _objects) {
        func(obj);
    }
}

inline bool ds2::scene::overlaping_layers(const object* a, const object* b)
{
    return a->layer_min() <= b->layer_max() && b->layer_min() <= a->layer_max();
}


