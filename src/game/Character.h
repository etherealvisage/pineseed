#ifndef PINESEED_GAME__CHARACTER_H
#define PINESEED_GAME__CHARACTER_H

#include <boost/shared_ptr.hpp>

#include "scene/SceneCharacter.fwd"

namespace Pineseed {
namespace Game {

class Character {
private:
    boost::shared_ptr<Scene::SceneCharacter> m_sceneCharacter;
public:
    Character();

    boost::shared_ptr<Scene::SceneCharacter> sceneCharacter() const
        { return m_sceneCharacter; }
    void setSceneCharacter(boost::shared_ptr<Scene::SceneCharacter> sc)
        { m_sceneCharacter = sc; }

    void update();
};

}  // namespace Game
}  // namespace Pineseed

#endif
