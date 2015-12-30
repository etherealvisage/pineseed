#include "PhysicalLocation.h"
#include "PhysicalForce.h"

#include "kriti/MessageSystem.h"

namespace Pineseed {
namespace Scene {

const double PhysicalLocation::s_physicsTick = 1e-3;
const int64_t PhysicalLocation::s_physicsTickNs = 1000000;

void PhysicalLocation::step(Kriti::TimeValue by) {
    Kriti::Math::Vector net;
    for(auto &force : m_forces) {
        if(force->enabled()) net += force->force();
    }

    Kriti::TimeValue nt = m_lastTick + by;
    double progress = nt.toUsec()/1e6;
    int ticks = static_cast<int>(std::floor(progress / s_physicsTick)
        + Kriti::Math::Constants::Epsilon);

    for(int i = 0; i < ticks; i ++) {
        Message3(Game, Debug, "tick!");
    }

    m_lastTick =
        m_lastTick + Kriti::TimeValue::fromNsec(s_physicsTickNs * ticks);
}

}  // namespace Scene
}  // namespace Pineseed
