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
#include<windows.h>

#include <opencv2/opencv.hpp>


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
    //scene.add_object(wall_1);

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

    std::shared_ptr<DebugObject> dob1(new DebugObject({ 350, 200 }));
    dob1->shape().add(cs);
    dob1->update_shape();
    dob1->set_mass(100e30);
    scene.add_object(dob1);
    objects.push_back(dob1);
    dob1->set_layer_range(1, 1);

    std::shared_ptr<DebugObject> dob2(new DebugObject({ 500, 200 }));
    dob2->shape().add(cs);
    dob2->update_shape();
    dob2->set_mass(100);
    scene.add_object(dob2);
    objects.push_back(dob2);
    dob2->set_layer_range(2, 2);

    std::shared_ptr<DebugObject> dob3(new DebugObject({ 650, 200 }));
    dob3->shape().add(cs);
    dob3->update_shape();
    dob3->set_mass(100);
    scene.add_object(dob3);
    objects.push_back(dob3);
    dob3->set_layer_range(3, 3);

    std::shared_ptr<DebugObject> dob4(new DebugObject({ 800, 200 }));
    dob4->shape().add(cs);
    dob4->update_shape();
    dob4->set_mass(100);
    scene.add_object(dob4);
    objects.push_back(dob4);
    dob4->set_layer_range(4, 4);

    ds2::fixed_joint j(dob1.get(), dob2.get(), vl::vec2d(75,0), vl::vec2d(-75,0));
    ds2::fixed_joint j2(dob2.get(), dob3.get(), vl::vec2d(75, 0), vl::vec2d(-75, 0));
    ds2::fixed_joint j3(dob3.get(), dob4.get(), vl::vec2d(75, 0), vl::vec2d(-75, 0));


    
    scene.add_joint(&j);
    scene.add_joint(&j2);
    scene.add_joint(&j3);


    while (window.isOpen())
    {
        //float dt = clock.restart().asSeconds();
        double dt = 1.0 / 30.0;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {           
                auto ctrlb = dob4;
                const double stren = 180000 / 1;
                if (event.key.code == sf::Keyboard::Up) 
                    ctrlb->apply_force(vl::vec2d(0,-stren),vl::vec2d(0,-10), dt);
                if (event.key.code == sf::Keyboard::Down)
                    ctrlb->apply_force(vl::vec2d(0, stren), vl::vec2d(0, 10), dt);
                if (event.key.code == sf::Keyboard::Right)
                    ctrlb->apply_force(vl::vec2d(stren, 0), vl::vec2d(10, 0), dt);
                if (event.key.code == sf::Keyboard::Left)
                    ctrlb->apply_force(vl::vec2d(-stren, 0), vl::vec2d(-10, 0), dt);
                if (event.key.code == sf::Keyboard::R)
                    ctrlb->rot() += 0.1;

                if (event.key.code == sf::Keyboard::H) {
                    ctrlb->apply_force_local({ 0, stren }, { 100,0 }, dt);
                    ctrlb->apply_force_local({ 0, -stren }, { -100,0 }, dt);
                }
            }
        }

        window.clear(sf::Color::White);
        //window.draw(sp);

        //dob->vel() += vl::vec2d(0, 160) * dt;
        //dob2->vel() += vl::vec2d(0, 160) * dt;
        //dob1->vel() += vl::vec2d(0, 160) * dt;
        //dob2->vel() += vl::vec2d(0, 160) * dt;
        //dob3->vel() += vl::vec2d(0, 160) * dt;
        dob4->vel() += vl::vec2d(0, 160) * dt;

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

        //utils::drawPoint(node1->global({ 100,0 }), window, sf::Color::Blue);
        //utils::drawPoint(node2->global({ -100,0 }), window, sf::Color::Blue);

        window.display();
    }

    return 0;
}