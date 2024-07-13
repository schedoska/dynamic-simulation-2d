#include <iostream>

#include "CircleDebug.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 800), "My window");
    window.setFramerateLimit(30);

    sf::Clock clock;
    CircleDebug c1(vl::vec2d(40, 60), 20);
   
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                /*
                if (event.key.code == sf::Keyboard::Up) obj2.setPosition(obj2.getPosition() + dm2::Vector(0, -9));
                if (event.key.code == sf::Keyboard::Down) obj2.setPosition(obj2.getPosition() + dm2::Vector(0, 9));
                if (event.key.code == sf::Keyboard::Right) obj2.setPosition(obj2.getPosition() + dm2::Vector(9, 0));
                if (event.key.code == sf::Keyboard::Left) obj2.setPosition(obj2.getPosition() + dm2::Vector(-9, 0));
                if (event.key.code == sf::Keyboard::R) obj2.setRotation(obj2.getRotation() + 0.2);
                */
            }
        }

        float dt = clock.restart().asSeconds();
        //std::cout << 1.0 / dt << "\n";
        window.clear(sf::Color::White);

        c1.draw(window);

        window.display();
    }

    return 0;
}