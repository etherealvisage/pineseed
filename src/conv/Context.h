#ifndef PINESEED_CONV__CONTEXT_H
#define PINESEED_CONV__CONTEXT_H

#include <string>
#include <map>

#include <boost/weak_ptr.hpp>

namespace Pineseed {
namespace Conv {

class Link;

class Context {
private:
    std::map<std::string, boost::shared_ptr<Link>> m_links;
    boost::weak_ptr<Context> m_parent;
public:
    Context();
    ~Context();

    void addLink(boost::shared_ptr<Link> link);
    boost::shared_ptr<Link> link(const std::string &name)
        { return m_links[name]; }

    boost::weak_ptr<Context> parent() const { return m_parent; }
    void setParent(boost::weak_ptr<Context> parent) { m_parent = parent; }
};

}  // namespace Conv
}  // namespace Pineseed

#endif
