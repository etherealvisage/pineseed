#ifndef PINESEED_LEVEL__GRID_H
#define PINESEED_LEVEL__GRID_H

#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "kriti/Resource.h"

#include "Platform.h"

namespace pugi {
class xml_node;
} // namespace pugi

namespace Pineseed {
namespace Level {

class Grid : public Kriti::Resource {
private:
    std::map<int, boost::shared_ptr<Platform>> m_platformMap;
    std::vector<boost::shared_ptr<Platform>> m_platforms;
public:
    virtual bool loadFrom(std::string name);

    /*void addPlatform(boost::shared_ptr<Platform> platform)
        { m_platforms.push_back(platform); }*/
private:
    void loadPlatform(int id, const pugi::xml_node &node);
};

}  // namespace Level
}  // namespace Pineseed

#endif
