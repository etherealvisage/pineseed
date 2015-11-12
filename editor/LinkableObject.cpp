#include "LinkableObject.h"
#include "Link.h"

LinkableObject::~LinkableObject() {
    while(m_links.length() > 0) {
        m_links.back()->deleteLater();
        removeLink(m_links.back());
    }
}
