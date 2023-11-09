#ifndef SQUARE_ENTITY_H
#define SQUARE_ENTITY_H

#include <gf/Vector.h>
#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <map>
#include "staticPlateforme.h"
#include "Input.cc"
#include <vector>

namespace hg
{
    class Square
    {
    public:
        Square(gf::Vector2f position, float size, gf::Color4f color, float gravity);

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
        void updateWithMap(float dt, std::map<int, StaticPlateform> plateforms, std::vector<Input> inputs);

        /**
        * Méthode permettant de faire les actions que l'on souhaite avec les inputs
        */
        void actionWithInputs(std::vector<Input> inputs);

        /*
        * Update the square
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
        float gravity;
    };
}

#endif // SQUARE_ENTITY_H