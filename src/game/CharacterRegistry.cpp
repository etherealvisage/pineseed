#include "CharacterRegistry.h"

namespace Pineseed {
namespace Game {

void CharacterRegistry::add(boost::shared_ptr<Character> character,
    std::string name) {

    m_characters[name] = character;
}

}  // namespace Game
}  // namespace Pineseed
