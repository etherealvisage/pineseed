#ifndef PINESEED_SCENE__BOX_SHAPE_H
#define PINESEED_SCENE__BOX_SHAPE_H

#include "PhysicalShape.h"
#include "ShapeVisitor.fwd"

#include "kriti/math/Vector.h"

namespace Pineseed {
namespace Scene {

class BoxShape : public PhysicalShape {
private:
    Kriti::Math::Vector m_origin;
    Kriti::Math::Vector m_size;
public:
    BoxShape(Kriti::Math::Vector origin, Kriti::Math::Vector size)
        : m_origin(origin), m_size(size) {}

    virtual void boundingBox(Kriti::Math::Vector &min,
        Kriti::Math::Vector &max) { min = m_origin; max = m_origin + m_size; }

    virtual bool intersects(const PhysicalShape &other) const;

    virtual void accept(const ShapeVisitor &visitor) const;
};

}  // namespace Scene
}  // namespace Pineseed

#endif
