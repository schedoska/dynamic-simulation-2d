#include <iostream>

#include "CircleDebug.h"
#include "ConvexDebug.h"
#include "scene.h"
#include "Utils.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 800), "My window");
    window.setFramerateLimit(20);

    sf::Clock clock;
    
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



    
    std::shared_ptr<ConvexDebug> v(new ConvexDebug(vl::vec2d(700, 195)));
    v->add_vertex(vl::vec2d(-100, -100));
    v->add_vertex(vl::vec2d(100, -100));
    v->add_vertex(vl::vec2d(100, 100));
    v->add_vertex(vl::vec2d(-100, 100));
    v->update_shape();
    v->scale(vl::vec2d(0.2, 1.8));
    v->mass() = 100;
    v->rot() = 3.14*99;
    v->vel() = vl::vec2d(-30, 0);
    v->inertia = (1600 + 129000) * 100 / 12;//800000;
    scene.add_object(v);



    std::shared_ptr<ConvexDebug> v2(new ConvexDebug(vl::vec2d(300, 550)));
    v2->add_vertex(vl::vec2d(-100, -100));
    v2->add_vertex(vl::vec2d(100, -100));
    v2->add_vertex(vl::vec2d(100, 100));
    v2->add_vertex(vl::vec2d(-100, 100));
    v2->update_shape();
    v2->scale(vl::vec2d(0.5, 0.5));
    v2->mass() = 100;
    v2->rot() = 3.14/4;
    v2->vel() = vl::vec2d(50, 0);
    v2->inertia = (20000) * 100 / 12;//800000;
    v2->rot_vel() = 4.2;
    scene.add_object(v2);
   
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up) conv1->pos() = conv1->pos() + vl::vec2d(0, -9);
                if (event.key.code == sf::Keyboard::Down) conv1->pos() = conv1->pos() + vl::vec2d(0, 9);
                if (event.key.code == sf::Keyboard::Right) conv1->pos() = conv1->pos() + vl::vec2d(9, 0);
                if (event.key.code == sf::Keyboard::Left) conv1->pos() = conv1->pos() + vl::vec2d(-9, 0);
                if (event.key.code == sf::Keyboard::R) conv1->rot() = conv1->rot() - 0.03;
            }
        }

        float dt = clock.restart().asSeconds();
        //std::cout << 1.0 / dt << "\n";
        window.clear(sf::Color::White);

        v->vel() += vl::vec2d(0, 160) * dt;
        v2->vel() += vl::vec2d(0, 160) * dt;
        scene.update(dt, window);
        for (const auto& i : scene.collisions()) {
            utils::drawLine(i.cp_a, i.cp_b, window, sf::Color::Red);
            utils::drawPoint(i.cp_a, window, sf::Color::Black);
            utils::drawPoint(i.cp_b, window, sf::Color::Black);
        }

        v->draw(window);
        v2->draw(window);
        conv1->draw(window);
        conv2->draw(window);
        conv3->draw(window);

        utils::drawPoint(v->pos(), window, sf::Color::Red);
        utils::drawPoint(conv1->pos(), window, sf::Color::Red);
        utils::drawPoint(conv2->pos(), window, sf::Color::Red);

        window.display();
    }

    return 0;
}