#ifndef LinkableObject_H
#define LinkableObject_H

#include <QList>
#include <QPointer>

#include "ConversationObject.h"
#include "Link.h"

class Link;

class LinkableObject : public ConversationObject {
private:
    QList<QPointer<Link>> m_links;
public:
    virtual ~LinkableObject();

    virtual void addLink(QPointer<Link> link) { m_links.push_back(link); }
    virtual void removeLink(Link *link) { m_links.removeAll(link); }

    QList<QPointer<Link>> &links() { return m_links; }
    const QList <QPointer<Link>> &links() const { return m_links; }
};

#endif
