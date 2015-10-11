#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsView>
#include <QMenu>
#include <QPainter>

#include "Node.h"
#include "moc_Node.cpp"

#include "Link.h"

Node::Node() {
    m_size = QSizeF(150, 200);

    setFlags(ItemIsSelectable);
}

QRectF Node::boundingRect() const {
    return QRectF(QPointF(0, 0), m_size);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *style,
    QWidget *widget) {

    QBrush b;
    b.setColor(QColor::fromRgb(240, 240, 240));
    b.setStyle(Qt::SolidPattern);
    painter->setBrush(b);
    painter->setBackgroundMode(Qt::OpaqueMode);
    painter->drawRect(QRectF(QPointF(0,0), m_size));
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton) {
        QPointF last = mapFromScene(event->lastScenePos());
        QPointF now = mapFromScene(event->scenePos());
        QPointF delta = now-last;

        setPos(pos() + delta);

        for(auto view : scene()->views()) {
            view->repaint();
        }
    }
}
