#include <QPainter>
#include <QGraphicsScene>

#include "Link.h"
#include "moc_Link.cpp"

#include "Node.h"

Link::Link(Node *from, Node *to) : m_from(from), m_to(to) {
    
}

QRectF Link::boundingRect() const {
    qreal minx = qMin(m_from->pos().x(), m_to->pos().x());
    qreal maxx = qMax(m_from->pos().x(), m_to->pos().x());
    qreal miny = qMin(m_from->pos().y(), m_to->pos().y());
    qreal maxy = qMax(m_from->pos().y(), m_to->pos().y());

    return QRectF(QPointF(minx, miny), QPointF(maxx, maxy)).normalized();
}

void Link::paint(QPainter *painter, const QStyleOptionGraphicsItem *style,
    QWidget *widget) {
    
    painter->drawLine(m_from->pos(), m_to->pos());

    painter->drawRect(boundingRect());
}
