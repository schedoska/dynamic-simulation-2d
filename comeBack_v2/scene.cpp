#include "scene.h"
#include "collision_solver.h"

void ds2::scene::add_object(const std::shared_ptr<circle_object>& circle){
	_circle_shapes.push_back(circle);
}

void ds2::scene::add_object(const std::shared_ptr<convex_object>& convex) {
	_convex_shapes.push_back(convex);
}

void ds2::scene::update(const double& dt, sf::RenderWindow& win){
    for (const auto& i : _circle_shapes)
        i->update(dt);
    for (const auto& i : _convex_shapes)
        i->update(dt);

    _collisions.clear();
    for (const auto& cir : _circle_shapes) {
        for (const auto& conv : _convex_shapes) {
            collision_data cd = collision_detection::check(conv, cir);
            if (cd.collides == false) 
                continue;
            collision_solver::solve_collision(cd, win);
            _collisions.push_back(cd);
        }
    }

    /*collision_data cd = collision_detection::check(_convex_shapes[0], _convex_shapes[1]);
    if (cd.collides == true) {
        collision_solver::solve_collision(cd, win);
        _collisions.push_back(cd);
    }*/

    collision_data cd = collision_detection::check(_convex_shapes[3], _convex_shapes[0]);
    if (cd.collides == true) {
        collision_solver::solve_collision(cd, win);
        _collisions.push_back(cd);
    }
    cd = collision_detection::check(_convex_shapes[3], _convex_shapes[1]);
    if (cd.collides == true) {
        collision_solver::solve_collision(cd, win);
        _collisions.push_back(cd);
    }
    cd = collision_detection::check(_convex_shapes[3], _convex_shapes[2]);
    if (cd.collides == true) {
        collision_solver::solve_collision(cd, win);
        _collisions.push_back(cd);
    }

     cd = collision_detection::check(_convex_shapes[4], _convex_shapes[0]);
    if (cd.collides == true) {
        collision_solver::solve_collision(cd, win);
        _collisions.push_back(cd);
    }
    cd = collision_detection::check(_convex_shapes[4], _convex_shapes[1]);
    if (cd.collides == true) {
        collision_solver::solve_collision(cd, win);
        _collisions.push_back(cd);
    }
    cd = collision_detection::check(_convex_shapes[4], _convex_shapes[2]);
    if (cd.collides == true) {
        collision_solver::solve_collision(cd, win);
        _collisions.push_back(cd);
    }

    cd = collision_detection::check(_convex_shapes[4], _convex_shapes[3]);
    if (cd.collides == true) {
        collision_solver::solve_collision(cd, win);
        _collisions.push_back(cd);
    }

    
}

const std::list<ds2::collision_data>& ds2::scene::collisions() const
{
	return _collisions;
}


