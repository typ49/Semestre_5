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

    // empeche le carré de traverser une plateforme
    void Square::collideWithPlateform(gf::Vector2f plateformPosition, float plateformHeight, float plateformLength)
    {
        // Supposons que la classe Square ait des membres m_position (position centrale du carré),
        // m_size (longueur d'un côté du carré), et m_velocity (vecteur de mouvement du carré)

        // Calculez les limites du carré en utilisant sa position centrale
        float squareLeft = m_position.x - m_size / 2;
        float squareRight = m_position.x + m_size / 2;
        float squareTop = m_position.y - m_size / 2;
        float squareBottom = m_position.y + m_size / 2;

        // Calculez les limites de la plateforme en utilisant sa position centrale
        float plateformLeft = plateformPosition.x - plateformLength / 2;
        float plateformRight = plateformPosition.x + plateformLength / 2;
        float plateformTop = plateformPosition.y - plateformHeight / 2;
        float plateformBottom = plateformPosition.y + plateformHeight / 2;

        // Vérifiez la collision
        if (squareRight > plateformLeft && squareLeft < plateformRight &&
            squareBottom > plateformTop && squareTop < plateformBottom)
        {

            // Collision détectée. Maintenant, nous devons ajuster la position du carré.

            // Vérifiez de quel côté le carré entre en collision
            float overlapLeft = squareRight - plateformLeft;
            float overlapRight = plateformRight - squareLeft;
            float overlapTop = squareBottom - plateformTop;
            float overlapBottom = plateformBottom - squareTop;

            // Trouvez le chevauchement le plus petit
            float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

            // Ajustez la position du carré en fonction du plus petit chevauchement
            if (minOverlap == overlapLeft)
            {
                m_position.x -= overlapLeft;
            }
            else if (minOverlap == overlapRight)
            {
                m_position.x += overlapRight;
            }
            else if (minOverlap == overlapTop)
            {
                m_position.y -= overlapTop;
            }
            else if (minOverlap == overlapBottom)
            {
                m_position.y += overlapBottom;
            }

            // Optionnellement, arrêtez le mouvement du carré lors de la collision
            m_velocity = gf::Vector2f(0, 0);
        }
    }

} // namespace hg
