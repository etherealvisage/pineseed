#ifndef Link_H
#define Link_H

#include <QGraphicsObject>

class Node;

class Link : public QGraphicsObject { Q_OBJECT
private:
    Node *m_from, *m_to;
public:
    Link(Node *from, Node *to);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);
};

#endif
