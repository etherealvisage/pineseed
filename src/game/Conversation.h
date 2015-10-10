#ifndef PINESEED_GAME__CONVERSATION_H
#define PINESEED_GAME__CONVERSATION_H

#include <boost/shared_ptr.hpp>

#include "conv/Node.h"

namespace Pineseed {
namespace Game {

class Conversation {
private:
    boost::shared_ptr<Conv::Node> m_begin, m_active;
public:
    void begin(boost::shared_ptr<Conv::Node> begin);

    void takeOption(int index);
private:
    void activate(boost::shared_ptr<Conv::Node> node);
};

}  // namespace Game
}  // namespace Pineseed

#endif
