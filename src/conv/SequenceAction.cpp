#include "SequenceAction.h"

namespace Pineseed {
namespace Conv {

void SequenceAction::trigger() {
    // TODO: make this sequential
    for(auto &action : m_actions) {
        action->trigger();
    }
}

}  // namespace Conv
}  // namespace Pineseed
