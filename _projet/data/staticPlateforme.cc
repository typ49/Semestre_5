#include "staticPlateforme.h"

namespace hg
{

    StaticPlateform::StaticPlateform(gf::Vector2f position, float height, float length, gf::Color4f color)
            : m_position(position), m_height(height), m_length(length), m_color(color)
        {
        }

    void StaticPlateform::render(gf::RenderTarget &target)
    {
        gf::RectangleShape shape({m_length, m_height});
        shape.setPosition(m_position);
        shape.setColor(m_color);
        shape.setAnchor(gf::Anchor::Center);
        target.draw(shape);
    }

    gf::Vector2f StaticPlateform::getPosition() const
    {
        return m_position;
    }

    float StaticPlateform::getHeight() const
    {
        return m_height;
    }

    float StaticPlateform::getLength() const
    {
        return m_length;
    }

    void StaticPlateform::setPosition (gf::Vector2f self) {
        m_position = self;
    }

    void StaticPlateform::setHeight(float self) {
        m_height = self;
    }

    void StaticPlateform::setLength(float self) {
        m_length = self;
    }

} // namespace hg
