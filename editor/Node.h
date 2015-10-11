#ifndef Node_H
#define Node_H

#include <QGraphicsObject>

class Node : public QGraphicsObject { Q_OBJECT
private:
    QSizeF m_size;
public:
    Node();

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

#endif
