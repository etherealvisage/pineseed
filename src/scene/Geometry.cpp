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

void Geometry::addGrid(boost::shared_ptr<Level::Grid> grid,
    Level::Pos offset) {

    auto result = boost::make_shared<Kriti::Render::Renderable>();

    for(auto &platform : grid->platforms()) {
        auto w = Kriti::Math::Vector(
            (platform->end() - platform->start()).x(), 0.0);
        auto h = Kriti::Math::Vector(
            0.0, (platform->end() - platform->start()).y());
        auto s = (platform->start() + offset);
        auto renderable = Kriti::Render::RenderableFactory().fromQuad(
            s, s+h, s+h+w, s+w, "simple");
        result->addRenderSequence(renderable->renderSequence(0));

        m_staticPlatforms.insert(platform->start() + offset,
            platform->end() + offset, platform);
    }
    m_renderables[grid] = result;
    m_container->add(result);
}

std::vector<boost::shared_ptr<Level::Platform>> Geometry::intersecting(
    Level::Pos min, Level::Pos max) {

    std::vector<boost::shared_ptr<Level::Platform>> results;

    m_staticPlatforms.search(min, max,
        decltype(m_staticPlatforms)::CallbackType(
            [&results](Level::Pos, Level::Pos,
                const boost::shared_ptr<Level::Platform> &platform) {

                results.push_back(platform);
            }));

    return results;
}

}  // namespace Scene
}  // namespace Pineseed
