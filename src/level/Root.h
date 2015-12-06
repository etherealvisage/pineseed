#ifndef PINESEED_LEVEL__ROOT_H
#define PINESEED_LEVEL__ROOT_H

#include <map>

#include <boost/shared_ptr.hpp>

#include "kriti/Resource.h"

namespace pugi {
class xml_node;
} // namespace pugi

namespace Pineseed {
namespace Level {

class Platform;

class Root : public Kriti::Resource {
private:
    std::map<int, boost::shared_ptr<Platform>> m_platformMap;
public:
    virtual bool loadFrom(std::string name);
private:
    void loadPlatform(int id, const pugi::xml_node &node);
};

}  // namespace Level
}  // namespace Pineseed

#endif
