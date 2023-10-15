#include "plateforme.h"

namespace hg
{

    class StaticPlateform
    {
    private:
        gf::Vector2f m_position;
        float m_size;
        gf::Color4f m_color;

    public:
        StaticPlateform(gf::Vector2f position, float size, gf::Color4f color)
            : m_position(position), m_size(size), m_color(color)
        {
        }

        void render(gf::RenderTarget &target)
        {
            gf::RectangleShape shape({m_size, m_size*1.5f});
            shape.setPosition(m_position);
            shape.setColor(m_color);
            shape.setAnchor(gf::Anchor::Center);
            target.draw(shape);
        }

        gf::Vector2f getPosition() const
        {
            return m_position;
        }

        float getSize() const
        {
            return m_size;
        }
    };
}