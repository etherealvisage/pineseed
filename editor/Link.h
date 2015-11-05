#ifndef Link_H
#define Link_H

#include <QPointer>

#include "ConversationObject.h"

class Node;

class Link : public ConversationObject { Q_OBJECT
private:
    int m_id;
    QPointer<Node> m_from, m_to;
    QString m_label;
    bool m_rtsLink;
    bool m_selected;
public:
    Link(Node *from, Node *to);
    virtual ~Link();

    virtual int id() const { return m_id; }

    virtual void select() { m_selected = true; update(); }
    virtual void deselect() { m_selected = false; update(); }

    Node *from() const { return m_from; }
    Node *to() const { return m_to; }
    const QString &label() const { return m_label; }
    bool isRtsLink() const { return m_rtsLink; }

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);

    virtual void edit(ConversationDataInterface *interface,
        ConversationData *data, QFormLayout *layout);
    virtual bool isSelection(QPointF point);

    virtual void serialize(QXmlStreamWriter &xml);
    virtual void deserialize(QDomElement &xml,
        const QMap<int, ConversationObject *> &objs, ConversationData *data);
private:
    QRectF labelBoundingRect() const;
private slots:
};

#endif
