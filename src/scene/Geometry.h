#ifndef PINESEED_SCENE__GEOMETRY_H
#define PINESEED_SCENE__GEOMETRY_H

#include <map>

#include <boost/shared_ptr.hpp>

#include "../level/Grid.fwd"

#include "kriti/render/Renderable.fwd"
#include "kriti/render/RenderableContainer.fwd"

namespace Pineseed {
namespace Scene {

class Geometry {
private:
    boost::shared_ptr<Kriti::Render::RenderableContainer> m_container;
    std::map<boost::shared_ptr<Level::Grid>,
        boost::shared_ptr<Kriti::Render::Renderable>> m_renderables;
public:
    Geometry();

    boost::shared_ptr<Kriti::Render::RenderableContainer> container() const
        { return m_container; }

    void addGrid(boost::shared_ptr<Level::Grid> grid);
};

}  // namespace Scene
}  // namespace Pineseed

#endif
