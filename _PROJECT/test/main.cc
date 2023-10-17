#include "squareEntity.h"
#include "plateforme.h"

#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/Vector.h>
#include <gf/Window.h>
#include <gf/Math.h>
#include <iostream>
#include <stdlib.h>

#define GRAVITY 100.0f

int main()
{
    // initialization
    static constexpr gf::Vector2i ScreenSize(500, 500);
    gf::Window window("It moves", ScreenSize);
    gf::RenderWindow renderer(window);

    // create a square
    hg::Square square({100.0f, 100.0f}, 20.0f, gf::Color::Red);

    // create a list of plateform
    hg::StaticPlateform plateformUp({250.0f, 0.0f}, 20.0f, 500.0f, gf::Color::Blue);
    hg::StaticPlateform plateformDown({250.0f, 500.0f}, 20.0f, 500.0f, gf::Color::Blue);
    hg::StaticPlateform plateformLeft({0.0f, 250.0f}, 500.0f, 20.0f, gf::Color::Blue);
    hg::StaticPlateform plateformRight({500.0f, 250.0f}, 500.0f, 20.0f, gf::Color::Blue);

    // game loop
    gf::Clock clock;
    renderer.clear(gf::Color::White);

    static constexpr float Speed = 100.0f;
    gf::Vector2f velocity(0, 0);

    while (window.isOpen())
    {

        gf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case gf::EventType::Closed:
                window.close();
                break;

            case gf::EventType::KeyPressed:
                switch (event.key.keycode)
                {
                case gf::Keycode::Escape:
                    window.close();
                    break;
                case gf::Keycode::Up:
                case gf::Keycode::Z:
                    velocity.y -= Speed;
                    break;
                case gf::Keycode::Down:
                case gf::Keycode::S:
                    velocity.y = Speed;
                    break;
                case gf::Keycode::Left:
                case gf::Keycode::Q:
                    velocity.x -= Speed;
                    break;
                case gf::Keycode::Right:
                case gf::Keycode::D:
                    velocity.x += Speed;
                    break;
                default:
                    break;
                }
                break;

            case gf::EventType::KeyReleased:
                switch (event.key.keycode)
                {
                case gf::Keycode::Up:
                case gf::Keycode::Z:
                    velocity.y += Speed;
                    break;
                case gf::Keycode::Down:
                case gf::Keycode::S:
                    velocity.y -= Speed;
                    break;
                case gf::Keycode::Left:
                case gf::Keycode::Q:
                    velocity.x += Speed;
                    break;
                case gf::Keycode::Right:
                case gf::Keycode::D:
                    velocity.x -= Speed;
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
        float dt = clock.restart().asSeconds();
        square.setVelocity(velocity);
        square.update(dt, plateformUp);
        square.update(dt, plateformDown);
        // square.update(dt, plateformLeft);
        // square.update(dt, plateformRight);

        // render
        renderer.clear(gf::Color::White);
        square.render(renderer);
        plateformUp.render(renderer);
        plateformDown.render(renderer);
        // plateformLeft.render(renderer);
        // plateformRight.render(renderer);
        renderer.display();
    }
    return 0;
}