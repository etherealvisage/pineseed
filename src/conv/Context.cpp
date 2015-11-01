#include "Context.h"

#include "Link.h"

namespace Pineseed {
namespace Conv {

Context::Context() {
    
}

Context::~Context() {
    
}

void Context::addLink(boost::shared_ptr<Link> link) {
    m_links[link->label()] = link;
}

}  // namespace Conv
}  // namespace Pineseed
