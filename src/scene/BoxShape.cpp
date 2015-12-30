#include "BoxShape.h"

#include "ShapeVisitor.h"

#include "kriti/math/Geometry.h"

namespace Pineseed {
namespace Scene {

bool BoxShape::intersects(const PhysicalShape &other) const {
    bool result = false;
    class Visitor : public ShapeVisitor {
    private:
        const BoxShape &m_box;
        bool &m_result;
    public:
        Visitor(const BoxShape &box, bool &result)
            : m_box(box), m_result(result) {}

        virtual void visit(const BoxShape &box) const {
            Kriti::Math::Vector s = m_box.m_origin;
            Kriti::Math::Vector e = s + m_box.m_size;
            Kriti::Math::Vector s2 = box.m_origin;
            Kriti::Math::Vector e2 = s + box.m_size;
            Kriti::Math::Geometry::intersectAARects(s, e, s2, e2);

            if((s - e).length2() < Kriti::Math::Constants::Epsilon) {
                m_result = true;
            }
        }
    };

    Visitor v(*this, result);
    other.accept(v);

    return result;
}

void BoxShape::accept(const ShapeVisitor &visitor) const {
    visitor.visit(*this);
}

}  // namespace Scene
}  // namespace Pineseed
