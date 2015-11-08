#ifndef LinkableObject_H
#define LinkableObject_H

#include <QList>
#include <QPointer>

#include "ConversationObject.h"

class Link;

class LinkableObject : public ConversationObject {
private:
    QList<QPointer<Link>> m_links;
public:
    virtual ~LinkableObject();

    QList<QPointer<Link>> &links() { return m_links; }
    const QList <QPointer<Link>> &links() const { return m_links; }

};

#endif
