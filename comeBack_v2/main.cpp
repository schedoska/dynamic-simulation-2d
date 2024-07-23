#include <iostream>

#include "CircleDebug.h"
#include "ConvexDebug.h"
#include "scene.h"
#include "Utils.h"
#include "joint.h"

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
    a->rot_vel() += (angle_diff * 750000000 - a->rot_vel() * 3000000) / a->inertia * dt;

    //std::cout << (vl::cross(vl::vec2d(0, -40), vl::vec2d(100, 0) * -0.4)
        // / a->inertia) * dt << "\n";

    vl::vec2d delta_vel = dv * (a->vel()).dot(dv);
    a->apply_force(dv * str * 38500 - delta_vel * 1950 * 1, a_loc, dt);
    std::cout << a->rot_vel() << " " << angle_diff << "\n";
}

int main()
{
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
    conv1->inertia = 1e30;
    scene.add_object(conv1);
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
    conv2->inertia = 1e30;
    scene.add_object(conv2);
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
    conv3->inertia = 1e30;
    scene.add_object(conv3);
    convexes.push_back(conv3);
    // ------------------------------------------------------------------------

    std::shared_ptr<ConvexDebug> v = utils::generate_rect(vl::vec2d(400, 500), vl::vec2d(100, 300), 100);
    scene.add_object(v);
    //v->rot() = 3.14 * 99;
    //v->vel() = vl::vec2d(-60, 0);
    convexes.push_back(v);

    std::shared_ptr<ConvexDebug> v2 = utils::generate_rect(vl::vec2d(800, 400), vl::vec2d(100, 100), 100);
    scene.add_object(v2);
    convexes.push_back(v2);

    std::shared_ptr<ConvexDebug> v3 = utils::generate_rect(vl::vec2d(550, 400), vl::vec2d(100, 100), 500);
    scene.add_object(v3);
    convexes.push_back(v3);

    std::shared_ptr<ConvexDebug> v4 = utils::generate_rect(vl::vec2d(800, 440.1), vl::vec2d(100, 100), 100);
    scene.add_object(v4);
    convexes.push_back(v4);


    ds2::spring_joint k(v4, v2, vl::vec2d(), vl::vec2d());
    k.stiff() = 1000;
    //scene.add_joint(&k);

    ds2::fixed_joint kk(v4, v2, vl::vec2d(0, -20), vl::vec2d(0, 20));
    scene.add_joint(&kk);


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
                const double stren = 180000/5;
                if (event.key.code == sf::Keyboard::Up) 
                    v2->apply_force(vl::vec2d(0,-stren),vl::vec2d(0,0), dt);
                if (event.key.code == sf::Keyboard::Down)
                    v2->apply_force(vl::vec2d(0, stren), vl::vec2d(0, 0), dt);
                if (event.key.code == sf::Keyboard::Right)
                    v2->apply_force(vl::vec2d(stren, 0), vl::vec2d(0, 0), dt);
                if (event.key.code == sf::Keyboard::Left)
                    v2->apply_force(vl::vec2d(-stren, 0), vl::vec2d(0, 0), dt);
            }
        }



        /*vl::vec2d force = vl::vec2d(1000, 0);
        v->pos() += (force / v->mass()) * dt;
        v->rot_vel() += (vl::cross(vl::vec2d(0,-100), force)
            / v->inertia) * dt;

        v->pos() += (force * -1.0 / v->mass()) * dt;
        v->rot_vel() += (vl::cross(vl::vec2d(0, -40), force * -1.0)
            / v->inertia) * dt;*/

        

        //std::cout << v2->pos() << v2->local(vl::vec2d(300, 250)) << "\n";

        //v2->apply_force(dd * 100 - v2->vel() * 10, vl::vec2d(50, 0), dt);
        //v2->rot_vel() -= 0.1 * v2->rot_vel();

        //solve_spring(v, v2, vl::vec2d(), vl::vec2d(), dt, 250);
        //solve_spring(v2, v3, vl::vec2d(), vl::vec2d(), dt, 150);
        //solve_spring(v2, v3, vl::vec2d(), vl::vec2d(), dt, 150);

        // v2 = vl::vec2d(550, 400)

        window.clear(sf::Color::White);

        solve_spring(v, vl::vec2d(), vl::vec2d(400, 500.1), dt, 0.1);
        double diff = 0.3 - v->rot_vel();
        v->rot_vel() += diff * 1;

        //std::cout << (v->global(vl::vec2d(0, -100)) - v2->global(vl::vec2d(-50, 0))).len() << "\n";
        //solve_spring(v, v2, vl::vec2d(0, -100), vl::vec2d(-50, 0), dt, 400);
        utils::drawLine(v->global(vl::vec2d(0, -100)), v2->global(vl::vec2d(-50, 0)), window, sf::Color::Red);

        //std::cout << v->rot_vel() << "\n";

        std::cout << v2->vel().len() << " " << v2->rot_vel() << "\n";

        /*solve_spring(v, vl::vec2d(), vl::vec2d(250, 250), dt, 150);
        solve_spring(v, v2, vl::vec2d(), vl::vec2d(), dt, 150);
        solve_spring(v2, v3, vl::vec2d(), vl::vec2d(), dt, 150);
        solve_spring(v3, v4, vl::vec2d(), vl::vec2d(), dt, 150);
        solve_spring(v4, vl::vec2d(), vl::vec2d(700, 250), dt, 150);

        utils::drawPoint(vl::vec2d(250, 250), window, sf::Color::Black);
        utils::drawPoint(v->pos(), window, sf::Color::Black);
        utils::drawPoint(v2->pos(), window, sf::Color::Black);
        utils::drawPoint(v3->pos(), window, sf::Color::Black);
        utils::drawPoint(v4->pos(), window, sf::Color::Black);
        utils::drawPoint(vl::vec2d(700, 250), window, sf::Color::Black);

        utils::drawLine(vl::vec2d(250, 250), v->pos(), window, sf::Color::Red);
        utils::drawLine(v->pos(), v2->pos(), window, sf::Color::Red);
        utils::drawLine(v2->pos(), v3->pos(), window, sf::Color::Red);
        utils::drawLine(v3->pos(), v4->pos(), window, sf::Color::Red);
        utils::drawLine(v4->pos(), vl::vec2d(700, 250), window, sf::Color::Red);*/




        //v->vel() += vl::vec2d(0, 160) * dt;
        v2->vel() += vl::vec2d(0, 160) * dt;
        v3->vel() += vl::vec2d(0, 160) * dt;
        //v4->vel() += vl::vec2d(0, 160) * dt;

        
        

        //v3->apply_force(vl::vec2d(-1000, 0), vl::vec2d(25, -25), dt);
        utils::drawPoint(v3->global(vl::vec2d(0, -40)), window, sf::Color::Black);


        scene.update(dt, window);
        for (const auto& i : scene.collisions()) {
            utils::drawLine(i.cp_a, i.cp_b, window, sf::Color::Red);
            utils::drawPoint(i.cp_a, window, sf::Color::Black);
            utils::drawPoint(i.cp_b, window, sf::Color::Black);
        }

        for (auto& i : convexes)
            i->draw(window);

        utils::drawPoint(v->pos(), window, sf::Color::Red);
        utils::drawPoint(conv1->pos(), window, sf::Color::Red);
        utils::drawPoint(conv2->pos(), window, sf::Color::Red);

        utils::drawPoint(vl::vec2d(300, 250), window, sf::Color::Black);
        //utils::drawLine(vl::vec2d(300, 250), v2->global(vl::vec2d(50, 0)), window, sf::Color::Red);

        window.display();
    }

    return 0;
}