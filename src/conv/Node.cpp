#include "Node.h"

namespace Pineseed {
namespace Conv {

void Node::triggerActions() {
    for(auto &action : m_actions) {
        action->trigger();
    }
}

}  // namespace Conv
}  // namespace Pineseed
