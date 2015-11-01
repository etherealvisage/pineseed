#include "ConcurrentAction.h"

namespace Pineseed {
namespace Conv {

void ConcurrentAction::trigger() {
    for(auto &action : m_actions) {
        action->trigger();
    }
}

}  // namespace Conv
}  // namespace Pineseed
