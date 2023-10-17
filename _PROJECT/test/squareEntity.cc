#include "squareEntity.h"
#include "plateforme.h"

#include <gf/Vector.h>
#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

#define GRAVITY 100.0f

namespace hg
{
    Square::Square(gf::Vector2f position, float size, gf::Color4f color)
        : m_position(position), m_velocity(0, 0), m_size(size), m_color(color)
    {
    }
    gf::Vector2f Square::getPosition() const
    {
        return m_position;
    }

    void Square::setVelocity(gf::Vector2f velocity)
    {
        m_velocity = velocity;
    }

    void Square::update(float dt, StaticPlateform plateform)
    {
        // Mettez à jour la position du carré
        m_position += dt * m_velocity;

        // Appliquez la gravité
        // setVelocity(m_velocity + gf::Vector2f(0, GRAVITY * dt));

        // Vérifiez les collisions avec la plateforme
        collideWithPlateform(plateform.getPosition(), plateform.getHeight(), plateform.getLength());
    }

    void Square::render(gf::RenderTarget &target)
    {
        gf::RectangleShape shape({m_size, m_size});
        shape.setPosition(m_position);
        shape.setColor(m_color);
        shape.setAnchor(gf::Anchor::Center);
        target.draw(shape);
    }

    void Square::collideWithPlateform(gf::Vector2f plateformPosition, float plateformHeight, float plateformLength)
    {
        // colision par le dessus
        if (m_position.y + m_size / 2 > plateformPosition.y - plateformHeight / 2 &&
            m_position.y + m_size / 2 < plateformPosition.y + plateformHeight / 2 &&
            m_position.x + m_size / 2 > plateformPosition.x - plateformLength / 2 &&
            m_position.x - m_size / 2 < plateformPosition.x + plateformLength / 2)
        {
            m_position.y = plateformPosition.y - plateformHeight / 2 - m_size / 2;
            m_velocity.y = 0;
        }
        // colision par le dessous
        if (m_position.y - m_size / 2 < plateformPosition.y + plateformHeight / 2 &&
            m_position.y - m_size / 2 > plateformPosition.y - plateformHeight / 2 &&
            m_position.x + m_size / 2 > plateformPosition.x - plateformLength / 2 &&
            m_position.x - m_size / 2 < plateformPosition.x + plateformLength / 2)
        {
            m_position.y = plateformPosition.y + plateformHeight / 2 + m_size / 2;
            m_velocity.y = 0;
        }
        // colision par la gauche
        if (m_position.x + m_size / 2 > plateformPosition.x - plateformLength / 2 &&
            m_position.x + m_size / 2 < plateformPosition.x + plateformLength / 2 &&
            m_position.y + m_size / 2 > plateformPosition.y - plateformHeight / 2 &&
            m_position.y - m_size / 2 < plateformPosition.y + plateformHeight / 2)
        {
            m_position.x = plateformPosition.x - plateformLength / 2 - m_size / 2;
            m_velocity.x = 0;
        }
        // colision par la droite
        if (m_position.x - m_size / 2 < plateformPosition.x + plateformLength / 2 &&
            m_position.x - m_size / 2 > plateformPosition.x - plateformLength / 2 &&
            m_position.y + m_size / 2 > plateformPosition.y - plateformHeight / 2 &&
            m_position.y - m_size / 2 < plateformPosition.y + plateformHeight / 2)
        {
            m_position.x = plateformPosition.x + plateformLength / 2 + m_size / 2;
            m_velocity.x = 0;
        }
    }
} // namespace hg
