#include <cstring>

#include <boost/make_shared.hpp>

#include "kriti/ResourceRegistry.h"
#include "kriti/MessageSystem.h"
#include "kriti/XMLResource.h"

#include "Grid.h"

namespace Pineseed {
namespace Level {

bool Grid::loadFrom(std::string name) {
    auto xml =
        Kriti::ResourceRegistry::get<Kriti::XMLResource>("platform/" + name);

    if(!xml) return false;

    auto objects = xml->doc().first_element_by_path("/platform/objects");

    // First pass to create objects.
    for(auto c = objects.first_child(); c ; c = c.next_sibling()) {
        int id = c.attribute("id").as_int();
        if(!std::strcmp(c.name(), "platform")) {
            m_platformMap[id] = boost::make_shared<Platform>();
        }
        else {
            Message3(Game, Debug, "Unknown object type " << c.name());
        }
    }

    // Second pass to parse objects.
    for(auto c = objects.first_child(); c ; c = c.next_sibling()) {
        int id = c.attribute("id").as_int();
        if(!std::strcmp(c.name(), "platform")) {
            loadPlatform(id, c);
        }
        else {
            Message3(Game, Debug, "Unknown object type " << c.name());
        }
    }

    for(auto p : m_platformMap) {
        m_platforms.push_back(p.second);
    }

    return true;
}

void Grid::loadPlatform(int id, const pugi::xml_node &node) {
    auto p = m_platformMap[id];

    Pos start;
    start.setX(node.attribute("x").as_double());
    start.setY(node.attribute("y").as_double());
    p->setStart(start);
    Pos size;
    size.setX(node.attribute("width").as_double());
    size.setY(node.attribute("height").as_double());
    p->setEnd(start + size);
}

}  // namespace Level
}  // namespace Pineseed
