#ifndef PINESEED_CONV__ROOT_H
#define PINESEED_CONV__ROOT_H

#include <map>

#include <boost/shared_ptr.hpp>

#include "kriti/Resource.h"

namespace pugi {
class xml_node;
} // namespace pugi

namespace Pineseed {
namespace Conv {

class Action;
class Context;
class Node;
class Link;

class Root : public Kriti::Resource {
private:
    std::map<int, boost::shared_ptr<Context>> m_contextMap;
    std::map<int, boost::shared_ptr<Node>> m_nodeMap;
    std::map<int, boost::shared_ptr<Link>> m_linkMap;
    boost::shared_ptr<Node> m_rootNode;
public:
    virtual bool loadFrom(std::string name);
private:
    void loadNode(int id, const pugi::xml_node &node);
    void loadContext(int id, const pugi::xml_node &node);
    void loadLink(int id, const pugi::xml_node &node);

    boost::shared_ptr<Action> loadAction(const pugi::xml_node &node);
};

}  // namespace Conv
}  // namespace Pineseed

#endif
