#include "squareEntity.h"
#include "staticPlateforme.h"

#include <gf/Vector.h>
#include <gf/Views.h>
#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <map>
#include <vector>
#include <iostream>

#define GRAVITY 100.0f
#define SPEED 200.0f

namespace hg
{
    Square::Square(gf::Vector2f position, float size, gf::Color4f color, float gravity)
        : m_position(position), m_velocity(0, 0), m_size(size), m_color(color), gravity(gravity)
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

    void Square::actionWithInputs(std::vector<Input> inputs) {
        if (inputs.empty()) return;      
        // On ne return pas à chaque action
        // Pourquoi ? Dans le cas où le joueur voudrait faire plusieurs actions dans une même frame
        // Par exemple : aller en haut à gauche

        // Évènement non released
        std::vector<Input>::iterator Z = std::find(inputs.begin(), inputs.end(), Input::Z);
        std::vector<Input>::iterator Up = std::find(inputs.begin(), inputs.end(), Input::Up);        
        if (Z != inputs.end() || Up != inputs.end()) {
            m_velocity.y -= SPEED;
        }
        std::vector<Input>::iterator D = std::find(inputs.begin(), inputs.end(), Input::D);
        std::vector<Input>::iterator Right = std::find(inputs.begin(), inputs.end(), Input::Right);
        if (D != inputs.end() || Right != inputs.end()) {
            m_velocity.x += SPEED;
        }
        std::vector<Input>::iterator S = std::find(inputs.begin(), inputs.end(), Input::S);
        std::vector<Input>::iterator Down = std::find(inputs.begin(), inputs.end(), Input::Down);
        if (S != inputs.end() || Down != inputs.end()) {
            m_velocity.y += SPEED;
        }
        std::vector<Input>::iterator Q = std::find(inputs.begin(), inputs.end(), Input::Q);
        std::vector<Input>::iterator Left = std::find(inputs.begin(), inputs.end(), Input::Left);
        if (Q != inputs.end() || Left != inputs.end()) { 
            m_velocity.x -= SPEED;
        }
        // Évènements released
        std::vector<Input>::iterator Z_Released = std::find(inputs.begin(), inputs.end(), Input::Z_Released);
        std::vector<Input>::iterator UpReleased = std::find(inputs.begin(), inputs.end(), Input::UpReleased);
        if (Z_Released != inputs.end() || UpReleased != inputs.end()) {
            m_velocity.y += SPEED;
        }
        std::vector<Input>::iterator D_Released = std::find(inputs.begin(), inputs.end(), Input::D_Released);
        std::vector<Input>::iterator RightReleased = std::find(inputs.begin(), inputs.end(), Input::RightReleased);
        if (D_Released != inputs.end() || RightReleased != inputs.end()) {
            m_velocity.x -= SPEED;
        }
        std::vector<Input>::iterator S_Released = std::find(inputs.begin(), inputs.end(), Input::S_Released);
        std::vector<Input>::iterator DownReleased = std::find(inputs.begin(), inputs.end(), Input::DownReleased);
        if (S_Released != inputs.end() || DownReleased != inputs.end()) {
            m_velocity.y -= SPEED;
        }
        std::vector<Input>::iterator Q_Released = std::find(inputs.begin(), inputs.end(), Input::Q_Released);
        std::vector<Input>::iterator LeftReleased = std::find(inputs.begin(), inputs.end(), Input::LeftReleased);
        if (Q_Released != inputs.end() || LeftReleased != inputs.end()) {
            m_velocity.x += SPEED;
        }        
    }

    void Square::updateWithMap(float dt, std::map<int, StaticPlateform> plateforms, std::vector<Input> inputs)
    {
        actionWithInputs(inputs);
        // Mettez à jour la position du carré
        m_position += dt * m_velocity;
        if (m_position.y < 0.0f) {
            m_position.y = 15.0f;
        }
        if (m_position.x < 0.0f) {
            m_position.x = 15.0f;
        }
        if (m_position.y > 200.0f) {
            m_position.y = 185.0f;
        }
        if (m_position.x > 200.0f) {
            m_position.x = 185.0f;
        }
        // Appliquez la gravité
        setVelocity(m_velocity + gf::Vector2f(0, gravity * dt));

        // Vérifiez les collisions avec la plateforme
        for (auto &plateform : plateforms)
        {
            collideWithPlateform(plateform.second.getPosition(), plateform.second.getHeight(), plateform.second.getLength());
        }

        
    }

    void Square::update(float dt, StaticPlateform plateform)
    {
        // Mettez à jour la position du carré
        m_position += dt * m_velocity;

        // Appliquez la gravité
        setVelocity(m_velocity + gf::Vector2f(0, gravity * dt));

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
            //m_velocity.y += -gravity;
        }
    }

} // namespace hg