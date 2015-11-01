#ifndef PINESEED_CONV__NODE_H
#define PINESEED_CONV__NODE_H

#include <vector>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace Pineseed {
namespace Conv {

class Action;
class SequenceAction;
class Context;

class Node {
private:
    boost::shared_ptr<SequenceAction> m_rootAction;

    boost::shared_ptr<Context> m_context;
public:
    Node();

    boost::shared_ptr<Context> context() const { return m_context; }

    void addAction(boost::shared_ptr<Action> action);
    void triggerActions();
};

}  // namespace Conv
}  // namespace Pineseed

#endif
