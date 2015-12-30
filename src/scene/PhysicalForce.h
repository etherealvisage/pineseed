#ifndef PINESEED_SCENE__PHYSICAL_FORCE_H
#define PINESEED_SCENE__PHYSICAL_FORCE_H

#include "kriti/math/Vector.h"

namespace Pineseed {
namespace Scene {

class PhysicalForce {
private:
    bool m_enabled;
    Kriti::Math::Vector m_force;
public:
    PhysicalForce(const Kriti::Math::Vector &force) : m_enabled(false),
        m_force(force) {}

    bool enabled() const { return m_enabled; }
    void enable() { m_enabled = true; }
    void disable() { m_enabled = false; }

    Kriti::Math::Vector force() const { return m_force; }
};

}  // namespace Scene
}  // namespace Pineseed

#endif
