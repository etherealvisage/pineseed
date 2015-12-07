#ifndef PINESEED_LEVEL__ROOT_H
#define PINESEED_LEVEL__ROOT_H

#include <map>

#include <boost/shared_ptr.hpp>

#include "kriti/Resource.h"

#include "Grid.h"

namespace pugi {
class xml_node;
} // namespace pugi

namespace Pineseed {
namespace Level {

class Platform;

class Root : public Kriti::Resource {
private:
    boost::shared_ptr<Grid> m_grid;
public:
    virtual bool loadFrom(std::string name);

    boost::shared_ptr<Grid> grid() const { return m_grid; }
private:
    void loadPlatform(int id, const pugi::xml_node &node);
};

}  // namespace Level
}  // namespace Pineseed

#endif
