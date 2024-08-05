#include <iostream>

#include "scene.h"
#include "Utils.h"
#include "joint.h"
#include "DebugObject.h"
#include "rectangle_shape.h"
#include "concave_shape.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <random>

#include <opencv2/opencv.hpp>

void solve_spring(std::shared_ptr<ds2::object> a, std::shared_ptr<ds2::object> b,
    vl::vec2d a_loc, vl::vec2d b_loc, double dt, double len)
{
    vl::vec2d dv = a->global(a_loc) - b->global(b_loc);
    double str = len - dv.len();
    dv.normalize();

    vl::vec2d delta_vel = dv * (a->vel() - b->vel()).dot(dv);

    a->apply_force(dv * str * 8500 - delta_vel * 950 * 1, a_loc, dt);
    b->apply_force(dv * -str * 8500 + delta_vel * 950 * 1, b_loc, dt);
}

void solve_spring(std::shared_ptr<ds2::object> a, vl::vec2d a_loc, vl::vec2d pt, double dt, double len)
{
    vl::vec2d dv = a->global(a_loc) - pt;
    double str = len - dv.len();
    dv.normalize();

    vl::vec2d delta_vel = dv * (a->vel()).dot(dv);

    //a->apply_force(dv * str * 18500 - delta_vel * 1650 * 1, a_loc, dt);
    a->apply_force(dv * str * 38500 - a->vel() * 1650 * 1, a_loc, dt);
    //b->apply_force(dv * -str * 4500 + delta_vel * 950 * 1, b_loc, dt);
}

void solve_fixed(std::shared_ptr<ds2::object> a, vl::vec2d a_loc, vl::vec2d pt, double angle, double dt)
{
    vl::vec2d dv = a->global(a_loc) - pt;
    double str = -dv.len();
    dv.normalize();
    

    double angle_diff = angle - a->rot();
    a->rot_vel() += (angle_diff * 750000000 - a->rot_vel() * 3000000) / a->inertia() * dt;

    //std::cout << (vl::cross(vl::vec2d(0, -40), vl::vec2d(100, 0) * -0.4)
        // / a->inertia) * dt << "\n";

    vl::vec2d delta_vel = dv * (a->vel()).dot(dv);
    a->apply_force(dv * str * 38500 - delta_vel * 1950 * 1, a_loc, dt);
    std::cout << a->rot_vel() << " " << angle_diff << "\n";
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 800), "My window");
    window.setFramerateLimit(30);

    sf::Clock clock;
    std::vector<std::shared_ptr<DebugObject>> objects;
    ds2::scene scene;

    ds2::convex_shape block;
    block.add(vl::vec2d(0, 0));
    block.add(vl::vec2d(50, 0));
    block.add(vl::vec2d(50, 500));
    block.add(vl::vec2d(0, 500));
    block.translate(vl::vec2d(-25, -250));

    std::shared_ptr<DebugObject> wall_1(new DebugObject(vl::vec2d(100, 400)));
    wall_1->shape().add(block);
    wall_1->update_shape();
    wall_1->set_mass(1e30);
    wall_1->inertia() = 1e30;
    objects.push_back(wall_1);
    scene.add_object(wall_1);

    std::shared_ptr<DebugObject> wall_2(new DebugObject(vl::vec2d(1000, 400)));
    wall_2->shape().add(block);
    wall_2->update_shape();
    wall_2->set_mass(1e30);
    wall_2->inertia() = 1e30;
    objects.push_back(wall_2);
    scene.add_object(wall_2);

    ds2::convex_shape block_2;
    block_2.add(vl::vec2d(0, 0));
    block_2.add(vl::vec2d(1000, 0));
    block_2.add(vl::vec2d(1000, 50));
    block_2.add(vl::vec2d(0, 50));
    block_2.translate(vl::vec2d(-500, -25));

    std::shared_ptr<DebugObject> wall_3(new DebugObject(vl::vec2d(550, 700)));
    wall_3->shape().add(block_2);
    wall_3->update_shape();
    wall_3->set_mass(1e30);
    wall_3->inertia() = 1e30;
    objects.push_back(wall_3);
    scene.add_object(wall_3);


    // -------------------------------------------------------------
    
    ds2::convex_shape cs;
    cs.add(vl::vec2d(-50, -50));
    cs.add(vl::vec2d(50, -50));
    cs.add(vl::vec2d(50, 50));
    cs.add(vl::vec2d(-50, 50));

    ds2::convex_shape cs2;
    cs2.add(vl::vec2d(-50, -50));
    cs2.add(vl::vec2d(50, -50));
    cs2.add(vl::vec2d(50, 50));
    cs2.add(vl::vec2d(-50, 50));
    cs2.translate(vl::vec2d(100, 0));

    ds2::convex_shape cs4;
    cs4.add(vl::vec2d(-50, -50));
    cs4.add(vl::vec2d(50, -50));
    cs4.add(vl::vec2d(50, 50));
    cs4.add(vl::vec2d(-50, 50));
    cs4.translate(vl::vec2d(100, 100));

    ds2::circle_shape cs3;
    cs3.radius() = 50;
    cs3.loc_pos() = vl::vec2d(-0, -50);

    std::shared_ptr<DebugObject> dob(new DebugObject(vl::vec2d(400, 300)));
    dob->set_mass(100);
    dob->inertia() = (100 * 100 + 100 * 100) * 500 / 12;
    dob->shape().add(cs);
    dob->shape().add(cs2);
    dob->shape().add(cs3);
    dob->shape().add(cs4);
    dob->shape().translate(vl::vec2d(-50, -50));
    dob->update_shape();
    dob->rot_vel() = 0.3;

    ds2::rectangle_shape rs(vl::vec2d(90, 190));
    //rs.set_size(vl::vec2d(30, 300));

    std::shared_ptr<DebugObject> dob2(new DebugObject(vl::vec2d(760, 500)));
    dob2->set_mass(100);
    dob2->inertia() = (100 * 100 + 100 * 100) * 1000 / 12;
    cs3.loc_pos() = vl::vec2d(0, 0);
    

    scene.add_object(dob);
    scene.add_object(dob2);
    objects.push_back(dob);
    objects.push_back(dob2);

    /*
    ds2::concave_shape s;
    s.add(vl::vec2d(-30, -100));
    s.add(vl::vec2d(30, -100));
    s.add(vl::vec2d(30, -30));
    s.add(vl::vec2d(90, -30));
    s.add(vl::vec2d(90, -100));
    s.add(vl::vec2d(120, -100));
    s.add(vl::vec2d(120, 100));
    s.add(vl::vec2d(-0, 100));
    //s.add(vl::vec2d(-120, -130));
    */

    ds2::concave_shape s;
    s.add(vl::vec2d(0, 0));
    s.add(vl::vec2d(100, 100));
    s.add(vl::vec2d(0, 100));

    //dob2->shape() = s.generate_shape_group(ds2::triangulation::delaunay);
    //dob2->shape().translate(vl::vec2d(-300, -150));


    //dob2->shape().translate_to_centroid();
    rs.translate({ 45,0 });
    dob2->shape().add(rs);
    rs.translate({ -90,0 });
    dob2->shape().add(rs);
    dob2->adjust_inertia();
    dob2->update_shape();
    std::cout << "AREA: " << dob2->shape().second_moment_area() / dob2->shape().area() << "\n";

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {                
                const double stren = 180000 / 4;
                if (event.key.code == sf::Keyboard::Up) 
                    dob2->apply_force(vl::vec2d(0,-stren),vl::vec2d(0,-10), dt);
                if (event.key.code == sf::Keyboard::Down)
                    dob2->apply_force(vl::vec2d(0, stren), vl::vec2d(0, 10), dt);
                if (event.key.code == sf::Keyboard::Right)
                    dob2->apply_force(vl::vec2d(stren, 0), vl::vec2d(10, 0), dt);
                if (event.key.code == sf::Keyboard::Left)
                    dob2->apply_force(vl::vec2d(-stren, 0), vl::vec2d(-10, 0), dt);
            }
        }

        window.clear(sf::Color::White);
        //window.draw(sp);

        dob->vel() += vl::vec2d(0, 160) * dt;
        dob2->vel() += vl::vec2d(0, 160) * dt;

        scene.update(dt, window);
        for (const auto& i : scene.collisions()) {
            utils::drawLine(i.data.cp_a, i.data.cp_b, window, sf::Color::Red);
            utils::drawPoint(i.data.cp_a, window, sf::Color::Black);
            utils::drawPoint(i.data.cp_b, window, sf::Color::Black);
        }

        for (auto& i : objects)
        {
            i->draw(window);
            utils::drawPoint(i->pos(), window, sf::Color::Red);

            double x = cos(i->rot()) * 0 - sin(i->rot()) * -1;
            double y = sin(i->rot()) * 0 + cos(i->rot()) * -1;
            vl::vec2d rot = vl::vec2d(x, y) * 20;
            utils::drawLine(i->pos(), i->pos() + rot, window, sf::Color::Magenta);
        }

        //for (auto i : pts) utils::drawPoint(i, window, sf::Color::Red);

        window.display();
    }

    return 0;
}