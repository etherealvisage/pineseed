#ifndef PINESEED_LEVEL__MOBILE_H
#define PINESEED_LEVEL__MOBILE_H

#include "Pos.h"

namespace Pineseed {
namespace Level {

class Mobile {
private:
    /// position of origin
    Pos m_position;

    /// bounding box information relative to origin
    Pos m_min, m_max;
public:
    Pos position() const { return m_position; }
    void updatePosition(Pos newPos) { m_position = newPos; }

    Pos min() const { return m_min; }
    Pos max() const { return m_max; }
};

}  // namespace Level
}  // namespace Pineseed

#endif
