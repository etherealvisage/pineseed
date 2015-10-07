#include "SpeechAction.h"

#include "game/UI.h"

namespace Pineseed {
namespace Conv {

void SpeechAction::trigger() {
    Game::UI::instance()->addJournalText(m_text);
}

}  // namespace Conv
}  // namespace Pineseed
