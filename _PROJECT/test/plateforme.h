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
        StaticPlateform(gf::Vector2f position, float size, gf::Color4f color);

        void render(gf::RenderTarget &target);

        gf::Vector2f getPosition() const;

        float getSize() const;

    private:
        gf::Vector2f m_position;
        float m_size;
        gf::Color4f m_color;
    };
}

#endif // PLATEFORME_H