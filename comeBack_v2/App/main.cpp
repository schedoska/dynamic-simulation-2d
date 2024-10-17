#include <iostream>

//#include "imgui.h"
//#include "imgui-SFML.h"

//#include "ds2/scene.h"
//#include "ds2/Utils.h"
//#include "ds2/joint.h"
//#include "ds2/DebugObject.h"
//#include "ds2/rectangle_shape.h"
//#include "ds2/concave_shape.h"

//#include <SFML/Window.hpp>
//#include <SFML/Graphics.hpp>
//#include <cstdlib>
//#include <random>
//#include <windows.h>

#include "App.h"
#include <nlohmann/json.hpp>
#include <fstream>

int main()
{
    sf::RenderWindow window;
    window.create(sf::VideoMode(1600, 900), "Dynamic symulation 2d", sf::Style::Default);

    window.setFramerateLimit(40);
    sf::Clock deltaClock;

    app a(&window);

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            }
            if (event.type == sf::Event::Resized) 
            {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        sf::Time dt = deltaClock.restart();

        window.clear(sf::Color(135, 126, 126));
        a.update(dt);
        a.draw();
        window.display();
    }
    return 0;
}
