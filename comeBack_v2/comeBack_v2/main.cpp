#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <iostream>
#include <numeric>
#include "Scene.h"
#include "DebugUI.h"
#include <chrono>
#include <cstdlib>
#include <time.h>

Object* createRandomObject()
{
    Object* obj = new Object(std::rand() % (900 + 100), std::rand() % (500 + 100));
    obj->addVertex(Math::Vector(0, 0));
    obj->addVertex(Math::Vector((rand() % 50) + 20, 0));
    obj->addVertex(Math::Vector((rand() % 50) + 20, (rand() % 50) + 20));
    obj->addVertex(Math::Vector(0, (rand() % 50) + 20));
    obj->setVelocity(Math::Vector(rand() % 110, rand() % 110));
    obj->setAngularVelocity(rand() % 3);
    obj->enableDebugDrawMode();
    return obj;
}

float FpsCumulative = 0;
int FpsSamplesCount = 0;

int main()
{
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode(1200, 800), "My window");
    //window.setFramerateLimit(500);

    // ------------------------------------------------------------------------------------------

    Object obj1(100,100);
    obj1.addVertex(Math::Vector(0, 0));
    obj1.addVertex(Math::Vector(60, 0));
    obj1.addVertex(Math::Vector(0, 60));
    obj1.setVelocity(Math::Vector(100, 70));
    obj1.enableDebugDrawMode();
    obj1.printInfo();
    std::cout << "\n\n";

    Object obj2(100, 100);
    obj2.addVertex(Math::Vector(0, 0));
    obj2.addVertex(Math::Vector(0, 50));
    obj2.addVertex(Math::Vector(50, 50));
    obj2.setVelocity(Math::Vector(90, 90));
    obj2.setAngularVelocity(1);
    obj2.enableDebugDrawMode();
    obj2.printInfo();
    std::cout << "\n\n";

    Scene scene;
    scene.setBouncingArea(BouncingRectangleArea(50, 50, 1100, 700));
    scene.enableBouncingArea();
    scene.addObject(&obj1);
    scene.addObject(&obj2);

    for (int i = 0; i < 30; i++)
    {
        scene.addObject(createRandomObject());
    }

    DebugUI debugUI(12, 12);

    // ------------------------------------------------------------------------------------------

    sf::Clock clock;
    sf::Clock fpsUpdateClock;
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();
        FpsCumulative += (1.0 / dt);
        FpsSamplesCount++;
        if (fpsUpdateClock.getElapsedTime().asSeconds() > 0.4)
        {
            debugUI.updateFPS(FpsCumulative / FpsSamplesCount);
            FpsCumulative = 0;
            FpsSamplesCount = 0;
            fpsUpdateClock.restart();
        }

        window.clear(sf::Color::Black);
        auto t1 = std::chrono::high_resolution_clock::now();
        scene.checkCollisions();
        auto t2 = std::chrono::high_resolution_clock::now();
        debugUI.updateSolverTime((t2 - t1).count() / 10e6);

        scene.update(dt);
        scene.debugDraw(window);

        debugUI.updateText();
        debugUI.draw(window);

        window.display();
    }

    return 0;
}