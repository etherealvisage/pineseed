#include "SceneCharacter.h"

#include "kriti/render/RenderableFactory.h"

namespace Pineseed {
namespace Scene {

SceneCharacter::SceneCharacter() {
    using V = Kriti::Math::Vector;
    m_renderable = Kriti::Render::RenderableFactory().fromQuad(
        V(0.0, 0.0), V(0.0, 1.0), V(1.0, 1.0), V(1.0, 0.0), "simple");

    m_renderable->scale() = 0.1;
}

Kriti::Math::Vector SceneCharacter::location() const {
    return m_renderable->location();
}

void SceneCharacter::setLocation(Kriti::Math::Vector location) {
    m_renderable->location() = location;
}


}  // namespace Scene
}  // namespace Pineseed
