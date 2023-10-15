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

        gf::Vector2f getPosition() const;

        void setVelocity(gf::Vector2f velocity);

        void update(float dt, StaticPlateform plateform);

        void render(gf::RenderTarget &target);

        void collideWithPlateform(gf::Vector2f plateformPosition, float plateformSize);

    private:
        gf::Vector2f m_position; // center of the square
        gf::Vector2f m_velocity;
        float m_size;
        gf::Color4f m_color;
    };
}

#endif // SQUARE_ENTITY_H