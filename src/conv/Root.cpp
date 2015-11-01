#include <cstring>

#include <boost/make_shared.hpp>

#include "kriti/ResourceRegistry.h"
#include "kriti/XMLResource.h"

#include "Root.h"
#include "Node.h"
#include "Link.h"
#include "Context.h"
#include "SequenceAction.h"
#include "ConcurrentAction.h"

namespace Pineseed {
namespace Conv {

bool Root::loadFrom(std::string name) {
    auto xml =
        Kriti::ResourceRegistry::get<Kriti::XMLResource>("conv/" + name);

    if(!xml) return false;

    auto objects = xml->doc().first_element_by_path("/conversation/objects");

    // First pass to create objects.
    for(auto c = objects.first_child(); c ; c = c.next_sibling()) {
        int id = c.attribute("id").as_int();
        if(!std::strcmp(c.name(), "node")) {
            m_nodeMap[id] = boost::make_shared<Node>();
        }
        else if(!std::strcmp(c.name(), "context")) {
            m_contextMap[id] = boost::make_shared<Context>();
        }
        else if(!std::strcmp(c.name(), "link")) {
            m_linkMap[id] = boost::make_shared<Link>();
        }
        else {
            Message3(Game, Debug, "Unknown object type " << c.name());
        }
    }

    // Second pass to parse objects.
    for(auto c = objects.first_child(); c ; c = c.next_sibling()) {
        int id = c.attribute("id").as_int();
        if(!std::strcmp(c.name(), "node")) {
            loadNode(id, c);
        }
        else if(!std::strcmp(c.name(), "context")) {
            loadContext(id, c);
        }
        else if(!std::strcmp(c.name(), "link")) {
            loadLink(id, c);
        }
        else {
            Message3(Game, Debug, "Unknown object type " << c.name());
        }
    }

    if(!m_rootNode) {
        Message3(Game, Error, "Couldn't load conversation \"" << name
            << "\": no root node");
        return false;
    }

    return true;
}

void Root::loadNode(int id, const pugi::xml_node &node) {
    auto n = m_nodeMap[id];

    if(!std::strcmp(node.attribute("entry").as_string(), "true"))
        m_rootNode = n;

    for(auto a = node.first_child(); ; a = a.next_sibling()) {
        auto action = loadAction(a);
        if(a == node.last_child()) break;
    }
}

void Root::loadContext(int id, const pugi::xml_node &node) {
    int parentID = node.attribute("parent").as_int(-1);
    if(parentID == -1) return;
    else m_contextMap[id]->setParent(m_contextMap[parentID]);
}

void Root::loadLink(int id, const pugi::xml_node &node) {
    auto l = m_linkMap[id];
    l->setNode(m_nodeMap[node.attribute("to").as_int()]);
    l->setLabel(node.attribute("label").as_string());
    // TODO: parse visible

    int fromID = node.attribute("from").as_int();
    if(m_contextMap.count(fromID)) {
        m_contextMap[fromID]->addLink(l);
    }
    else {
        m_nodeMap[fromID]->context()->addLink(l);
    }
}

boost::shared_ptr<Action> Root::loadAction(const pugi::xml_node &node) {
    // XXX: This needs to be kept synchronized with the editor
    enum NodeType {
        Empty,
        Speech,
        Emote,
        Sequence,
        Concurrent,
        Conditional,
        Jump,
        EndConversation
    };

    NodeType type = (NodeType)node.attribute("type").as_int();

    switch(type) {
    case Empty:
        Message3(Game, Debug, "Empty action found!");
        return boost::shared_ptr<Action>();
    case Speech:
        /*Message3(Game, Debug, "Speech action found! ["
            << node.attribute("speaker").as_string() << "]: "
            << node.attribute("speech").as_string());*/
        return boost::shared_ptr<Action>();
    case Emote:
        return boost::shared_ptr<Action>();
    case Sequence: {
        auto ret = boost::make_shared<SequenceAction>();

        for(auto n = node.first_child(); n; n = n.next_sibling()) {
            ret->addAction(loadAction(n));
        }
        return ret;
    }
    case Concurrent: {
        auto ret = boost::make_shared<ConcurrentAction>();

        for(auto n = node.first_child(); n; n = n.next_sibling()) {
            ret->addAction(loadAction(n));
        }
        return ret;
    }
    case Conditional:
    case Jump:
    case EndConversation:
        break;
    }

    return boost::shared_ptr<Action>();
}

}  // namespace Conv
}  // namespace Pineseed
