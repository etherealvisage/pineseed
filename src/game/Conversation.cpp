#include "kriti/MessageSystem.h"

#include "Conversation.h"

#include "UI.h"

namespace Pineseed {
namespace Game {

void Conversation::begin(boost::shared_ptr<Conv::Node> begin) {
    m_begin = begin;
    activate(begin);
}

void Conversation::takeOption(int index) {
    /*if(index >= m_active->links().size()) return;

    activate(m_active->links()[index].node());*/
}

void Conversation::activate(boost::shared_ptr<Conv::Node> node) {
    m_active = node;
    UI::instance()->clearOptions();

    Message3(Game, Debug, "Adding conversation options...");
    /*for(auto &link : node->links()) {
        UI::instance()->addOption(link.label());
        Message3(Game, Debug, "Added option \"" << link.label() << "\"!");
    }*/

    node->triggerActions();
}

}  // namespace Game
}  // namespace Pineseed
