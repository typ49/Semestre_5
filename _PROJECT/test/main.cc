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
namespace hg
{
    int main()
    {
        // initialization
        static constexpr gf::Vector2i ScreenSize(500, 500);
        gf::Window window("It moves", ScreenSize);
        gf::RenderWindow renderer(window);

        // create a square
        hg::Square square({100.0f, 100.0f}, 20.0f, gf::Color::Red);

        // create a plateform
        hg::StaticPlateform plateform({100.0f, 400.0f}, 100.0f, gf::Color::Blue);

        // game loop
        gf::Clock clock;
        renderer.clear(gf::Color::White);

        static constexpr float Speed = 100.0f;
        gf::Vector2f velocity(0, GRAVITY);

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
                        velocity.y = -Speed;
                        break;
                    case gf::Keycode::Down:
                    case gf::Keycode::S:
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
                        velocity.y = GRAVITY;
                        break;
                    case gf::Keycode::Down:
                    case gf::Keycode::S:
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
            square.update(clock.restart().asSeconds(), plateform);

            // render
            renderer.clear(gf::Color::White);
            square.render(renderer);
            plateform.render(renderer);
            renderer.display();
        }
        return 0;
    }
} // namespace hg