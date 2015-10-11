#ifndef Node_H
#define Node_H

#include <QGraphicsObject>

class Link;

class Node : public QGraphicsObject { Q_OBJECT
private:
    QSizeF m_size;
    QVector<Link *> m_links;
public:
    Node();

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);

    void addLink(Link *link) { m_links.push_back(link); }
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif
