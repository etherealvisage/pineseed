#ifndef PINESEED_CONV__SPEECH_ACTION_H
#define PINESEED_CONV__SPEECH_ACTION_H

#include "game/Character.h"

#include "Action.h"

namespace Pineseed {
namespace Conv {

class SpeechAction : public Action {
private:
    boost::shared_ptr<Game::Character> m_character;
    std::string m_text;
public:
    SpeechAction(boost::shared_ptr<Game::Character> character,
        std::string text) : m_character(character), m_text(text) {}

    virtual void trigger();
};

}  // namespace Conv
}  // namespace Pineseed

#endif
