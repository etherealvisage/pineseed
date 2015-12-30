#ifndef PINESEED_SCENE__SHAPE_VISITOR_H
#define PINESEED_SCENE__SHAPE_VISITOR_H

#include "BoxShape.fwd"

namespace Pineseed {
namespace Scene {

class ShapeVisitor {
public:
    virtual ~ShapeVisitor() {}

    virtual void visit(const BoxShape &box) const = 0;
};

}  // namespace Scene
}  // namespace Pineseed

#endif
