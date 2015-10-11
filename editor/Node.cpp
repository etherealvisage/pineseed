#include <QPainter>

#include "Node.h"
#include "moc_Node.cpp"

Node::Node() {
    m_size = QSizeF(150, 200);

    setFlags(ItemIsMovable | ItemIsSelectable);
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

void Node::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    qDebug("Context menu time!");
}
