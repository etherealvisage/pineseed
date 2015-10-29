#ifndef Link_H
#define Link_H

#include "ConversationObject.h"

class Node;

class Link : public ConversationObject { Q_OBJECT
private:
    Node *m_from, *m_to;
public:
    Link(Node *from, Node *to);
    virtual ~Link();

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);

    virtual void edit(QFormLayout *layout);
private slots:
    void labelUpdated(const QString &newLabel);
};

#endif
