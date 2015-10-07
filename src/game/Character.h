#ifndef PINESEED_GAME__CHARACTER_H
#define PINESEED_GAME__CHARACTER_H

#include <boost/shared_ptr.hpp>

#include "kriti/render/Renderable.h"

namespace Pineseed {
namespace Game {

class Character {
private:
    boost::shared_ptr<Kriti::Render::Renderable> m_speechBubble;
public:
    Character();

    boost::shared_ptr<Kriti::Render::Renderable> speechBubble;
};

}  // namespace Game
}  // namespace Pineseed

#endif
