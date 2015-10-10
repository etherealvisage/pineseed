#ifndef PINESEED_CONV__NODE_H
#define PINESEED_CONV__NODE_H

#include <vector>

#include <boost/function.hpp>

#include "Action.h"

namespace Pineseed {
namespace Conv {

class Node {
public:
    class Link {
    private:
        boost::shared_ptr<Node> m_node;
        std::string m_label;
        boost::function<bool ()> m_visible;
    public:
        Link(boost::shared_ptr<Node> node, std::string label,
            boost::function<bool ()> visible = boost::function<bool ()>())
            : m_node(node), m_label(label), m_visible(visible) {}

        boost::shared_ptr<Node> node() { return m_node; }
        std::string label() const { return m_label; }
        bool visible() { if(m_visible) return m_visible(); return true; }
    };
private:
    std::vector<boost::shared_ptr<Action>> m_actions;

    std::vector<Link> m_links;
public:
    void addAction(boost::shared_ptr<Action> action)
        { m_actions.push_back(action); }
    void triggerActions();

    void addLink(Link link) { m_links.push_back(link); }
    std::vector<Link> &links() { return m_links; }
    const std::vector<Link> &links() const { return m_links; }
};

}  // namespace Conv
}  // namespace Pineseed

#endif
