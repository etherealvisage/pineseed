#ifndef PINESEED_LEVEL__GRID_H
#define PINESEED_LEVEL__GRID_H

#include <vector>

#include "Platform.h"

namespace Pineseed {
namespace Level {

class Grid {
private:
    std::vector<Platform> m_platforms;
public:
    void addPlatform(Platform &platform);
};

}  // namespace Level
}  // namespace Pineseed

#endif
