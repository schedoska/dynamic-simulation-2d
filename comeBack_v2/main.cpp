#include <iostream>

#include "CircleDebug.h"
#include "ConvexDebug.h"
#include "scene.h"
#include "Utils.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>

void solve_spring(std::shared_ptr<ds2::object> a, std::shared_ptr<ds2::object> b,
    vl::vec2d a_loc, vl::vec2d b_loc, double dt, double len)
{
    vl::vec2d dv = a->global(a_loc) - b->global(b_loc);
    double str = len - dv.len();
    dv.normalize();

    //a->apply_force(dv * str * 4500 - a->vel() * 950, a_loc, dt);
    b->apply_force(dv * -str * 4500 - b->vel() * 950, b_loc, dt);

    std::cout << str << "\n";
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

    std::shared_ptr<ConvexDebug> conv3(new ConvexDebug(vl::vec2d(900, 699)));
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


    std::shared_ptr<ConvexDebug> v = utils::generate_rect(vl::vec2d(550, 580), vl::vec2d(100, 100), 100);
    scene.add_object(v);
    v->rot() = 3.14 * 99;
    v->vel() = vl::vec2d(-60, 0);
    convexes.push_back(v);

    std::shared_ptr<ConvexDebug> v2 = utils::generate_rect(vl::vec2d(300, 580), vl::vec2d(100, 100), 100);
    scene.add_object(v2);
    convexes.push_back(v2);


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
                const double stren = 180000;
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

        vl::vec2d dd = vl::vec2d(300, 250) - v2->global(vl::vec2d(50, 0));
        double d = v2->vel().dot(dd);

        //std::cout << v2->pos() << v2->local(vl::vec2d(300, 250)) << "\n";

        //v2->apply_force(dd * 100 - v2->vel() * 10, vl::vec2d(50, 0), dt);
        //v2->rot_vel() -= 0.1 * v2->rot_vel();

        solve_spring(v, v2, vl::vec2d(), vl::vec2d(), dt, 250);


        window.clear(sf::Color::White);

        //v->vel() += vl::vec2d(0, 160) * dt;
        //v2->vel() += vl::vec2d(0, 160) * dt;

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
        utils::drawLine(vl::vec2d(300, 250), v2->global(vl::vec2d(50, 0)), window, sf::Color::Red);

        window.display();
    }

    return 0;
}