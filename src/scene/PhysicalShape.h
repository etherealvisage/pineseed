#ifndef PINESEED_SCENE__PHYSICAL_SHAPE_H
#define PINESEED_SCENE__PHYSICAL_SHAPE_H

#include "ShapeVisitor.fwd"

#include "kriti/math/Vector.fwd"

namespace Pineseed {
namespace Scene {

class PhysicalShape {
public:
    virtual ~PhysicalShape() {}

    virtual void boundingBox(Kriti::Math::Vector &min,
        Kriti::Math::Vector &max) = 0;

    virtual bool intersects(const PhysicalShape &otherShape) const = 0;

    virtual void accept(const ShapeVisitor &visitor) const = 0;
};

}  // namespace Scene
}  // namespace Pineseed

#endif
