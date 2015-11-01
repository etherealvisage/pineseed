#ifndef PINESEED_CONV__LINK_H
#define PINESEED_CONV__LINK_H

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

namespace Pineseed {
namespace Conv {

class Node;

class Link {
private:
    boost::shared_ptr<Node> m_node;
    std::string m_label;
    boost::function<bool ()> m_visible;
public:
    Link() {}
    Link(boost::shared_ptr<Node> node, std::string label,
        boost::function<bool ()> visible = boost::function<bool ()>())
        : m_node(node), m_label(label), m_visible(visible) {}

    boost::shared_ptr<Node> node() { return m_node; }
    void setNode(boost::shared_ptr<Node> node) { m_node = node; }
    std::string label() const { return m_label; }
    void setLabel(std::string label) { m_label = label; }
    bool visible() { if(m_visible) return m_visible(); return true; }
    void setVisible(boost::function<bool ()> visible) { m_visible = visible; }
};

}  // namespace Conv
}  // namespace Pineseed

#endif
