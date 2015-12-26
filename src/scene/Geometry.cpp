#include <boost/make_shared.hpp>

#include "Geometry.h"

#include "../level/Grid.h"

#include "kriti/render/RenderableContainer.h"
#include "kriti/render/RenderableFactory.h"

#include "kriti/MessageSystem.h"

namespace Pineseed {
namespace Scene {

Geometry::Geometry() {
    m_container = boost::make_shared<Kriti::Render::RenderableContainer>();
}

void Geometry::addGrid(boost::shared_ptr<Level::Grid> grid) {
    auto result = boost::make_shared<Kriti::Render::Renderable>();

    for(auto &platform : grid->platforms()) {
        auto w = Kriti::Math::Vector(
            (platform->end() - platform->start()).x() / 400.0, 0.0);
        auto h = Kriti::Math::Vector(
            0.0, (platform->end() - platform->start()).y() / 400.0);
        auto s = platform->start() / 400.0;
        auto renderable = Kriti::Render::RenderableFactory().fromQuad(
            s, s+h, s+h+w, s+w, "simple");
        result->addRenderSequence(renderable->renderSequence(0));
    }
    m_renderables[grid] = result;
    m_container->add(result);
}

}  // namespace Scene
}  // namespace Pineseed
