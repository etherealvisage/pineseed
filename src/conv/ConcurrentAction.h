#ifndef PINESEED_CONV__CONCURRENT_ACTION_H
#define PINESEED_CONV__CONCURRENT_ACTION_H

#include <vector>

#include "Action.h"

namespace Pineseed {
namespace Conv {

class ConcurrentAction : public Action {
private:
    std::vector<boost::shared_ptr<Action>> m_actions;
public:
    ConcurrentAction() {}

    void addAction(boost::shared_ptr<Action> action)
        { m_actions.push_back(action); }

    virtual void trigger();
};

}  // namespace Conv
}  // namespace Pineseed

#endif
