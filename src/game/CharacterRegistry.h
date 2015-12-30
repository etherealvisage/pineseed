#ifndef PINESEED_GAME__CHARACTER_REGISTRY_H
#define PINESEED_GAME__CHARACTER_REGISTRY_H

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>

namespace Pineseed {
namespace Game {

class Character;

class CharacterRegistry {
private:
    std::map<std::string, boost::shared_ptr<Character>> m_characters;
public:
    void add(boost::shared_ptr<Character> character, std::string name);
};

}  // namespace Game
}  // namespace Pineseed

#endif
