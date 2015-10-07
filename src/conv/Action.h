#ifndef PINESEED_CONV__ACTION_H
#define PINESEED_CONV__ACTION_H

#include <boost/shared_ptr.hpp>

#include "game/Character.h"

namespace Pineseed {
namespace Conv {

class Action {
public:
    virtual ~Action() {}

    virtual void trigger() = 0;
};

}  // namespace Conv
}  // namespace Pineseed

#endif
