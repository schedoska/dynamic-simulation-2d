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

    std::shared_ptr<ConvexDebug> conv1(new ConvexDebug(vl::vec2d(160, 100)));
    conv1->add_vertex(vl::vec2d(-20, -20));
    conv1->add_vertex(vl::vec2d(-20, 80));
    conv1->add_vertex(vl::vec2d(55, -20));
    conv1->update_shape();
    scene.add_object(conv1);
    std::shared_ptr<ConvexDebug> conv2(new ConvexDebug(vl::vec2d(300, 200)));
    conv2->add_vertex(vl::vec2d(-20, -20));
    conv2->add_vertex(vl::vec2d(-20, 80));
    conv2->add_vertex(vl::vec2d(55, -20));
    conv2->rot_vel() = 2 * 3.14 / 5;
    conv2->update_shape();
    scene.add_object(conv2);
    
    std::shared_ptr<CircleDebug> v(new CircleDebug(vl::vec2d(100, 100), 20));
    v->vel() = vl::vec2d(10, 0);

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
                if (event.key.code == sf::Keyboard::R) conv1->rot() = conv1->rot() + 0.03;
            }
        }

        float dt = clock.restart().asSeconds();
        //std::cout << 1.0 / dt << "\n";
        window.clear(sf::Color::White);

        scene.update(dt);
        for (const auto& i : scene.collisions()) {
            utils::drawLine(i.cp_a, i.cp_b, window, sf::Color::Red);
            utils::drawPoint(i.cp_a, window, sf::Color::Black);
            utils::drawPoint(i.cp_b, window, sf::Color::Black);
        }


        v->draw(window);
        conv1->draw(window);
        conv2->draw(window);

        utils::drawPoint(v->pos(), window, sf::Color::Red);
        utils::drawPoint(conv1->pos(), window, sf::Color::Red);
        utils::drawPoint(conv2->pos(), window, sf::Color::Red);

        window.display();
    }

    return 0;
}