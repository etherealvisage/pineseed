#ifndef Link_H
#define Link_H

#include "ConversationObject.h"

class Node;

class Link : public ConversationObject { Q_OBJECT
private:
    Node *m_from, *m_to;
    QString m_label;
    bool m_selected;
public:
    Link(Node *from, Node *to);
    virtual ~Link();

    virtual void select() { m_selected = true; update(); }
    virtual void deselect() { m_selected = false; update(); }

    Node *from() const { return m_from; }
    Node *to() const { return m_to; }
    const QString &label() const { return m_label; }

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);

    virtual void edit(QFormLayout *layout);
    virtual bool isSelection(QPointF point);

    virtual void serialize(QXmlStreamWriter &xml,
        const QMap<ConversationObject *, int> &itemID);
    virtual void deserialize(QDomElement &xml,
        const QMap<int, ConversationObject *> &objs);
private:
    QRectF labelBoundingRect() const;
private slots:
};

#endif
