#ifndef PINESEED_SCENE__SCENE_CHARACTER_H
#define PINESEED_SCENE__SCENE_CHARACTER_H

#include <boost/shared_ptr.hpp>

#include "kriti/math/Vector.fwd"

#include "kriti/render/Renderable.fwd"
#include "kriti/render/RenderableContainer.fwd"

namespace Pineseed {
namespace Scene {

class SceneCharacter {
private:
    boost::shared_ptr<Kriti::Render::Renderable> m_renderable;
public:
    SceneCharacter();

    boost::shared_ptr<Kriti::Render::Renderable> renderable() const
        { return m_renderable; }

    Kriti::Math::Vector location() const;
    void setLocation(Kriti::Math::Vector location);
};

}  // namespace Scene
}  // namespace Pineseed

#endif
