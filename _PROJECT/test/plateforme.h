#ifndef PLATEFORME_H
#define PLATEFORME_H

#include <gf/Vector.h>
#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

namespace hg
{
    class StaticPlateform
    {
    public:
        StaticPlateform(gf::Vector2f position, float height, float length, gf::Color4f color);

        void render(gf::RenderTarget &target);

        gf::Vector2f getPosition() const;

        float getHeight() const;

        float getLength() const;

    private:
        gf::Vector2f m_position; // Centre du carré
        float m_height;
        float m_length;
        gf::Color4f m_color;
    };
}

#endif // PLATEFORME_H