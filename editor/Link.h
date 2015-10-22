#ifndef Link_H
#define Link_H

#include <QGraphicsObject>

#include "ConversationObject.h"

class Node;

class Link : public ConversationObject { Q_OBJECT
private:
    Node *m_from, *m_to;
    QString m_label;
public:
    Link(Node *from, Node *to);

    Node *from() const { return m_from; }
    Node *to() const { return m_to; }

    QString label() const { return m_label; }
    void setLabel(QString label) { m_label = label; }

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);
    QRectF labelBoundingRect() const;
private:
    void calculateBestLine(QPointF &from, QPointF &to) const;
};

#endif
