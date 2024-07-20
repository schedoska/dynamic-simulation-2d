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
    scene.add_object(conv1);

    std::shared_ptr<ConvexDebug> conv2(new ConvexDebug(vl::vec2d(100, 695)));
    conv2->add_vertex(vl::vec2d(0, 0));
    conv2->add_vertex(vl::vec2d(100, 0));
    conv2->add_vertex(vl::vec2d(100, 100));
    conv2->add_vertex(vl::vec2d(0, 100));
    conv2->update_shape();
    conv2->scale(vl::vec2d(0.5, -4));
    conv2->mass() = 1e20;
    conv2->rot() = 0.0001;
    scene.add_object(conv2);

    std::shared_ptr<ConvexDebug> conv3(new ConvexDebug(vl::vec2d(900, 695)));
    conv3->add_vertex(vl::vec2d(0, 0));
    conv3->add_vertex(vl::vec2d(100, 0));
    conv3->add_vertex(vl::vec2d(100, 100));
    conv3->add_vertex(vl::vec2d(0, 100));
    conv3->update_shape();
    conv3->scale(vl::vec2d(0.5, -4));
    conv3->mass() = 1e20;
    conv3->rot() = 0.4;
    scene.add_object(conv3);



    
    std::shared_ptr<ConvexDebug> v(new ConvexDebug(vl::vec2d(960, 195)));
    v->add_vertex(vl::vec2d(-100, -100));
    v->add_vertex(vl::vec2d(100, -100));
    v->add_vertex(vl::vec2d(100, 100));
    v->add_vertex(vl::vec2d(-100, 100));
    v->update_shape();
    v->scale(vl::vec2d(0.2, 0.4));
    v->mass() = 100;
    v->rot() = 7.2;
    v->vel() = vl::vec2d(0, 0);
    scene.add_object(v);

    scene.add_object(v);
   
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

        v->vel() += vl::vec2d(0, 1.5);
        scene.update(dt, window);
        for (const auto& i : scene.collisions()) {
            utils::drawLine(i.cp_a, i.cp_b, window, sf::Color::Red);
            utils::drawPoint(i.cp_a, window, sf::Color::Black);
            utils::drawPoint(i.cp_b, window, sf::Color::Black);
        }


        v->draw(window);
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