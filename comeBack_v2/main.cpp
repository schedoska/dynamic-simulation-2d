#include <iostream>

#include "CircleDebug.h"
#include "ConvexDebug.h"
#include "scene.h"
#include "Utils.h"
#include "joint.h"
#include "DebugObject.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>

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
    int k = 2 > 6 ? 1 : 99;
    std::cout << k << "\n";

    sf::RenderWindow window(sf::VideoMode(1200, 800), "My window");
    window.setFramerateLimit(20);

    sf::Clock clock;
    std::vector<std::shared_ptr<ConvexDebug>> convexes;
    ds2::scene scene;

    std::shared_ptr<ConvexDebug> conv1(new ConvexDebug(vl::vec2d(100, 700)));
    conv1->add_vertex(vl::vec2d(0, 0));
    conv1->add_vertex(vl::vec2d(100, 0));
    conv1->add_vertex(vl::vec2d(100, 100));
    conv1->add_vertex(vl::vec2d(0, 100));
    conv1->update_shape();
    conv1->scale(vl::vec2d(10, 0.5));
    conv1->mass() = 1e20;
    conv1->rot() = 0.0001;
    conv1->inertia() = 1e30;
    //scene.add_object(conv1);
    convexes.push_back(conv1);

    std::shared_ptr<ConvexDebug> conv2(new ConvexDebug(vl::vec2d(100, 699)));
    conv2->add_vertex(vl::vec2d(0, 0));
    conv2->add_vertex(vl::vec2d(100, 0));
    conv2->add_vertex(vl::vec2d(100, 100));
    conv2->add_vertex(vl::vec2d(0, 100));
    conv2->update_shape();
    conv2->scale(vl::vec2d(0.5, -4));
    conv2->mass() = 1e20;
    conv2->rot() = 0.0001;
    conv2->inertia() = 1e30;
    //scene.add_object(conv2);
    convexes.push_back(conv2);

    std::shared_ptr<ConvexDebug> conv3(new ConvexDebug(vl::vec2d(900, 1699)));
    conv3->add_vertex(vl::vec2d(0, 0));
    conv3->add_vertex(vl::vec2d(100, 0));
    conv3->add_vertex(vl::vec2d(100, 100));
    conv3->add_vertex(vl::vec2d(0, 100));
    conv3->update_shape();
    conv3->scale(vl::vec2d(0.5, -4));
    conv3->mass() = 1e20;
    conv3->rot() = 0;// 3.14 / 3.0;
    conv3->inertia() = 1e30;
    //scene.add_object(conv3);
    convexes.push_back(conv3);
    // ------------------------------------------------------------------------

    std::shared_ptr<ConvexDebug> v = utils::generate_rect(vl::vec2d(400, 500), vl::vec2d(100, 300), 100);
    //scene.add_object(v);
    convexes.push_back(v);

    std::shared_ptr<ConvexDebug> v2 = utils::generate_rect(vl::vec2d(800, 400), vl::vec2d(100, 100), 100);
    scene.add_object(v2);
    convexes.push_back(v2);

    
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

    std::shared_ptr<DebugObject> dob(new DebugObject(vl::vec2d(500, 300)));
    dob->mass() = 1000;
    dob->inertia() = (100 * 100 + 100 * 100) * 1000 / 12;
    dob->shape().add(cs);
    dob->shape().add(cs2);
    dob->shape().add(cs3);
    dob->shape().add(cs4);
    dob->update_shape();

    std::shared_ptr<DebugObject> dob2(new DebugObject(vl::vec2d(660, 600)));
    dob2->mass() = 100;
    dob2->inertia() = (100 * 100 + 100 * 100) * 100 / 12;
    dob2->shape().add(cs3);
    dob2->update_shape();

    scene.add_object(dob);
    scene.add_object(dob2);

    //dob.rot() = 3.14 / 4;

    
    



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
                const double stren = 180000 / 10;
                if (event.key.code == sf::Keyboard::Up) 
                    dob2->apply_force(vl::vec2d(0,-stren),vl::vec2d(0,0), dt);
                if (event.key.code == sf::Keyboard::Down)
                    dob2->apply_force(vl::vec2d(0, stren), vl::vec2d(0, 0), dt);
                if (event.key.code == sf::Keyboard::Right)
                    dob2->apply_force(vl::vec2d(stren, 0), vl::vec2d(0, 0), dt);
                if (event.key.code == sf::Keyboard::Left)
                    dob2->apply_force(vl::vec2d(-stren, 0), vl::vec2d(0, 0), dt);

                /*const double speed = 10;
                if (event.key.code == sf::Keyboard::Up)
                    dob2->pos() += vl::vec2d(0, -speed);
                if (event.key.code == sf::Keyboard::Down)
                    dob2->pos() += vl::vec2d(0, speed);
                if (event.key.code == sf::Keyboard::Right)
                    dob2->pos() += vl::vec2d(speed, 0);
                if (event.key.code == sf::Keyboard::Left)
                    dob2->pos() += vl::vec2d(-speed, 0);
                if (event.key.code == sf::Keyboard::R)
                    dob2->rot() += 0.1;*/

            }
        }



        

        window.clear(sf::Color::White);

        solve_spring(v, vl::vec2d(), vl::vec2d(400, 500.1), dt, 0.1);
        double diff = 0.3 - v->rot_vel();
        v->rot_vel() += diff * 1;

        //v->vel() += vl::vec2d(0, 160) * dt;
        //v2->vel() += vl::vec2d(0, 160) * dt;

        //dob->vel() += vl::vec2d(0, 160) * dt;
        //dob2->vel() += vl::vec2d(0, -160) * dt;


        ds2::object_collision_data ocd = ds2::collision_detection_2::check(dob2, dob);

        //dob->rot() = -0.5;
        //dob2->rot() = 0.01;
        dob->draw(window);
        dob2->draw(window);

        utils::drawLine(ocd.data.cp_a, ocd.data.cp_b, window, sf::Color::Magenta);

        

        scene.update(dt, window);
        for (const auto& i : scene.collisions()) {
            utils::drawLine(i.data.cp_a, i.data.cp_b, window, sf::Color::Red);
            utils::drawPoint(i.data.cp_a, window, sf::Color::Black);
            utils::drawPoint(i.data.cp_b, window, sf::Color::Black);
        }

        for (auto& i : convexes)
            i->draw(window);

        utils::drawPoint(v->pos(), window, sf::Color::Red);
        utils::drawPoint(conv1->pos(), window, sf::Color::Red);
        utils::drawPoint(conv2->pos(), window, sf::Color::Red);

        utils::drawPoint(vl::vec2d(300, 250), window, sf::Color::Black);
        //utils::drawLine(vl::vec2d(300, 250), v2->global(vl::vec2d(50, 0)), window, sf::Color::Red);

        utils::drawPoint(dob->pos(), window, sf::Color::Red);

        window.display();
    }

    return 0;
}