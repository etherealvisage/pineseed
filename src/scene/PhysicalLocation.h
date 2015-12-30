#ifndef PINESEED_SCENE__PHYSICAL_LOCATION_H
#define PINESEED_SCENE__PHYSICAL_LOCATION_H

#include <vector>

#include <boost/shared_ptr.hpp>

#include "kriti/TimeValue.h"
#include "kriti/math/Vector.h"

namespace Pineseed {
namespace Scene {

class PhysicalForce;
class PhysicalShape;

class PhysicalLocation {
private:
    static const double s_physicsTick;
    static const int64_t s_physicsTickNs;
private:
    Kriti::Math::Vector m_location;
    boost::shared_ptr<PhysicalShape> m_shape;
    std::vector<boost::shared_ptr<PhysicalForce>> m_forces;
    bool m_hasMass;
    Kriti::Math::Vector m_velocity;

    Kriti::TimeValue m_lastTick;
public:
    PhysicalLocation();
    Kriti::Math::Vector location() const { return m_location; }
    boost::shared_ptr<PhysicalShape> shape() const { return m_shape; }

    void addForce(boost::shared_ptr<PhysicalForce> force)
        { m_forces.push_back(force); }

    bool hasMass() const { return m_hasMass; }

    void step(Kriti::TimeValue by);
};

}  // namespace Scene
}  // namespace Pineseed

#endif
