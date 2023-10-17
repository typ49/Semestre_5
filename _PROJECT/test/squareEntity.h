#ifndef SQUARE_ENTITY_H
#define SQUARE_ENTITY_H

#include <gf/Vector.h>
#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include "plateforme.h"

namespace hg
{
    class Square
    {
    public:
        Square(gf::Vector2f position, float size, gf::Color4f color);

        /**
         * @brief Récupère la position du carré
        */
        gf::Vector2f getPosition() const;

        /**
         * @brief Définit la vitesse du carré
        */
        void setVelocity(gf::Vector2f velocity);

        /**
         * @brief Met à jour la position du carré
        */
        void update(float dt, StaticPlateform plateform);

        /**
         * @brief Affiche le carré
        */
        void render(gf::RenderTarget &target);


        /**
         * @brief Gère les collisions avec la plateforme
        */
        void collideWithPlateform(gf::Vector2f plateformPosition, float plateformHeight, float plateformLength);

    private:
        gf::Vector2f m_position; // Centre du carré
        gf::Vector2f m_velocity;
        float m_size;
        gf::Color4f m_color;
    };
}

#endif // SQUARE_ENTITY_H