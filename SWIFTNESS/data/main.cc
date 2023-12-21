#include "squareEntity.h"
#include "staticPlateforme.h"

#include <gf/Clock.h>
#include <gf/Views.h>
#include <gf/View.h>
#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Vector.h>
#include <gf/Window.h>
#include <gf/Math.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

#define GRAVITY 100.0f
#define SPEED 200.0f

int main()
{
    // initialize the window
    gf::Window window("Swiftness", {480, 320});
    
    // set the window in fullscreen mode
    gf::RenderWindow renderer(window);

    // Get the screen size after setting fullscreen mode
    gf::Vector2i ScreenSize = window.getSize();

    // create a vector of Input
    std::vector<Input> enumVector;

    // create a square (ps : camera on 100.0f ; 100.0f to center the camera on the square)
    //hg::Square square({100.0f, 100.0f}, 20.0f, gf::Color::Red, GRAVITY);

    // create a list of wall
    std::map<int, hg::StaticPlateform> wall;
    hg::StaticPlateform wallUp({240.0f, 0.0f}, 10.0f, 480.0f, gf::Color::Blue);
    hg::StaticPlateform wallDown({240.0f, 320.0f}, 10.0f, 480.0f, gf::Color::Blue);
    hg::StaticPlateform wallLeft({0.0f, 160.0f}, 320.0f, 10.0f, gf::Color::Blue);
    hg::StaticPlateform wallRight({480.0f, 160.0f}, 320.0f, 10.0f, gf::Color::Blue);
    wall.insert(std::pair<int, hg::StaticPlateform>(0, wallUp));
    wall.insert(std::pair<int, hg::StaticPlateform>(1, wallDown));
    wall.insert(std::pair<int, hg::StaticPlateform>(2, wallLeft));
    wall.insert(std::pair<int, hg::StaticPlateform>(3, wallRight));

    // create a list of plateform
    std::map<int, hg::StaticPlateform> plateform;
    hg::StaticPlateform plateformOneVertical({240.0f, 270.0f}, 5.0f, 50.0f, gf::Color::Green);


    gf::ExtendView camera({240.0f, 160.0f}, {480.0f, 320.0f});

    // game loop
    gf::Clock clock;
    renderer.clear(gf::Color::White);

    //gf::Vector2f velocity(0, GRAVITY);
    bool isresize = false;
    while (window.isOpen())
    {

        gf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case gf::EventType::Closed:
                enumVector.push_back(Input::Closed);
                //window.close();
                break;
            
            case gf::EventType::Resized:
                enumVector.push_back(Input::Resized);
                /*isresize = true;
                ScreenSize = window.getSize();
                plateformUp.setPosition({static_cast<float>(ScreenSize.width) / 2.0f, 0.0f});
                plateformUp.setLength(static_cast<float>(ScreenSize.width));
                plateformDown.setPosition({static_cast<float>(ScreenSize.width) / 2.0f, static_cast<float>(ScreenSize.height)});
                plateformDown.setLength(static_cast<float>(ScreenSize.width));
                plateformLeft.setPosition({0.0f, static_cast<float>(ScreenSize.height) / 2.0f});
                plateformLeft.setHeight(static_cast<float>(ScreenSize.height));
                plateformRight.setPosition({static_cast<float>(ScreenSize.width), static_cast<float>(ScreenSize.height) / 2.0f});
                plateformRight.setHeight(static_cast<float>(ScreenSize.height));
                plateform.erase(0);
                plateform.erase(1);
                plateform.erase(2);
                plateform.erase(3);
                plateform.insert(std::pair<int, hg::StaticPlateform>(0, plateformUp));
                plateform.insert(std::pair<int, hg::StaticPlateform>(1, plateformDown));
                plateform.insert(std::pair<int, hg::StaticPlateform>(2, plateformLeft));
                plateform.insert(std::pair<int, hg::StaticPlateform>(3, plateformRight));*/
                break;
            case gf::EventType::KeyPressed:
                switch (event.key.keycode)
                {
                case gf::Keycode::Escape:
                    enumVector.push_back(Input::Escape);
                    //window.close();
                    break;
                case gf::Keycode::Up:
                    enumVector.push_back(Input::Up);
                    //velocity.y -= SPEED;
                    break;
                case gf::Keycode::Z:
                    enumVector.push_back(Input::Z);
                    //velocity.y -= SPEED;
                    break;
                case gf::Keycode::Down:
                    enumVector.push_back(Input::Down);
                    //velocity.y += SPEED;
                    break;
                case gf::Keycode::S:
                    enumVector.push_back(Input::S);
                    //velocity.y += SPEED;
                    break;
                case gf::Keycode::Left:
                    enumVector.push_back(Input::Left);
                    //velocity.x -= SPEED;
                    break;
                case gf::Keycode::Q:
                    enumVector.push_back(Input::Q);
                    //velocity.x -= SPEED;
                    break;
                case gf::Keycode::Right:
                    enumVector.push_back(Input::Right);
                    //velocity.x += SPEED;
                    break;
                case gf::Keycode::D:
                    enumVector.push_back(Input::D);
                    //velocity.x += SPEED;
                    break;
                default:
                    break;
                }
                break;

            case gf::EventType::KeyReleased:
                switch (event.key.keycode)
                {
                case gf::Keycode::Up:
                    enumVector.push_back(Input::UpReleased);
                    //velocity.y += SPEED;
                    break;
                case gf::Keycode::Z:
                    enumVector.push_back(Input::Z_Released);
                    //velocity.y += SPEED;
                    break;
                case gf::Keycode::Down:
                    enumVector.push_back(Input::DownReleased);
                    //velocity.y -= SPEED;
                    break;
                case gf::Keycode::S:
                    enumVector.push_back(Input::S_Released);
                    //velocity.y -= SPEED;
                    break;
                case gf::Keycode::Left:
                    enumVector.push_back(Input::LeftReleased);
                    //velocity.x += SPEED;
                    break;
                case gf::Keycode::Q:
                    enumVector.push_back(Input::Q_Released);
                    //velocity.x += SPEED;
                    break;
                case gf::Keycode::Right:
                    enumVector.push_back(Input::RightReleased);
                    //velocity.x -= SPEED;
                    break;
                case gf::Keycode::D:
                    enumVector.push_back(Input::D_Released);
                    //velocity.x -= SPEED;
                    break;
                default:
                    break;
                }
                break;

            default:
                break;
            }
        }

        // update the square
        std::vector<Input>::iterator it1 = std::find(enumVector.begin(), enumVector.end(), Input::Escape);
        std::vector<Input>::iterator it2 = std::find(enumVector.begin(), enumVector.end(), Input::Closed);
        // Si on a pressé une de ces deux touches, on ferme la fenêtre de jeux
        if (it1 != enumVector.end() || it2 != enumVector.end()) window.close();
        float dt = clock.restart().asSeconds();
        // square.setVelocity(velocity);
        // square.update(dt, plateformUp);
        // square.update(dt, plateformDown);
        // square.update(dt, plateformLeft);
        // square.update(dt, plateformRight);
        // square.updateWithMap(dt, plateform, enumVector);
        enumVector.clear();
        if (isresize) {
            isresize = false;
            gf::RenderWindow renderer(window);
        }
        // render
        renderer.clear(gf::Color::White);
        renderer.setView(camera);
        // square.render(renderer);
        wallUp.render(renderer);
        wallDown.render(renderer);
        wallLeft.render(renderer);
        wallRight.render(renderer);
        plateformOneVertical.render(renderer);
        renderer.display();
    }
    return 0;
}