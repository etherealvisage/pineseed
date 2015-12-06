#ifndef PINESEED_LEVEL__PLATFORM_H
#define PINESEED_LEVEL__PLATFORM_H

#include "Pos.h"

namespace Pineseed {
namespace Level {

class Platform {
private:
    Pos m_start, m_end;
public:
    Platform();

    Pos start() const { return m_start; }
    void setStart(const Pos &pos) { m_start = pos; }
    Pos end() const { return m_end; }
    void setEnd(const Pos &pos) { m_end = pos; }
};

}  // namespace Level
}  // namespace Pineseed

#endif
