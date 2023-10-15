#include "squareEntity.h"
#include "plateforme.h"

#define GRAVITY 100.0f

namespace hg
{

public:
    Square(gf::Vector2f position, float size, gf::Color4f color)
        : m_position(position), m_velocity(0, 0), m_size(size), m_color(color)
    {
    }

    gf::Vector2f getPosition() const
    {
        return m_position;
    }

    void setVelocity(gf::Vector2f velocity)
    {
        m_velocity = velocity;
    }

    void update(float dt, StaticPlateform plateform)
    {
        // update the position of the square
        m_position += dt * m_velocity;

        // apply gravity
        setVelocity(m_velocity + gf::Vector2f(0, GRAVITY * dt));

        // check collisions with the plateform
        collideWithPlateform(plateform.getPosition(), plateform.getSize());
    }

    void render(gf::RenderTarget &target)
    {
        gf::RectangleShape shape({m_size, m_size});
        shape.setPosition(m_position);
        shape.setColor(m_color);
        shape.setAnchor(gf::Anchor::Center);
        target.draw(shape);
    }

    void collideWithPlateform(gf::Vector2f plateformPosition, float plateformSize)
    {
        // if the square is above the plateform
        if (m_position.y - m_size / 2 > plateformPosition.y + plateformSize / 2)
        {
            // do nothing
        }
        else
        {
            // if the square is on the left of the plateform
            if (m_position.x + m_size / 2 < plateformPosition.x - plateformSize / 2)
            {
                // do nothing
            }
            // if the square is on the right of the plateform
            else if (m_position.x - m_size / 2 > plateformPosition.x + plateformSize / 2)
            {
                // do nothing
            }
            // if the square is on the plateform
            else
            {
                // if the square is above the plateform
                if (m_position.y - m_size / 2 < plateformPosition.y + plateformSize / 2)
                {
                    // if the square is falling
                    if (m_velocity.y > 0)
                    {
                        // stop the square
                        m_velocity.y = 0;
                        // put the square on the plateform
                        m_position.y = plateformPosition.y + plateformSize / 2 + m_size / 2;
                    }
                }
            }
        }
    }

private:
    gf::Vector2f m_position; // center of the square
    gf::Vector2f m_velocity;
    float m_size;
    gf::Color4f m_color;
};
} // namespace hg