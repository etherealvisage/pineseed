#include <boost/make_shared.hpp>

#include "Node.h"
#include "Context.h"
#include "SequenceAction.h"

namespace Pineseed {
namespace Conv {

Node::Node() {
    m_context = boost::make_shared<Context>();
    m_rootAction = boost::make_shared<SequenceAction>();
}

void Node::addAction(boost::shared_ptr<Action> action) {
    m_rootAction->addAction(action);
}

void Node::triggerActions() {
    m_rootAction->trigger();
}

}  // namespace Conv
}  // namespace Pineseed
