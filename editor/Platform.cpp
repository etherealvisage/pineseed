#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <cmath>

#include "Platform.h"

Platform::Platform(const QRectF &rect) {
    m_size = rect.size();
    setPos(rect.topLeft());
    setFlags(ItemIsSelectable | ItemIsMovable);
    m_id = -1;
    m_selected = false;
}

bool Platform::isSelection(QPointF point) {
    return boundingRect().contains(point);
}

QRectF Platform::boundingRect() const {
    // accommodate borders
    const qreal border = 1.0;
    return QRectF(QPointF(0,0), m_size + QSizeF(border*2, border*2));
}

void Platform::paint(QPainter *painter, const QStyleOptionGraphicsItem *style,
    QWidget *widget) {

    QBrush b;
    if(m_selected) b.setColor(QColor::fromRgb(200, 200, 255));
    else b.setColor(QColor::fromRgb(240, 240, 240));
    b.setStyle(Qt::SolidPattern);
    painter->setBrush(b);

    QPointF snappedPos = pos();
    snappedPos.setX(std::floor(snappedPos.x()/25.0)*25);
    snappedPos.setY(std::floor(snappedPos.y()/25.0)*25);
    QSizeF snappedSize = m_size;
    snappedSize.setWidth(std::floor(m_size.width()/25.0)*25);
    snappedSize.setHeight(std::floor(m_size.height()/25.0)*25);

    painter->drawRect(QRectF(snappedPos - pos(), snappedSize));
}

void Platform::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    auto ctrl = (event->modifiers() & Qt::ControlModifier) != 0;
    // movement?
    if(!ctrl && (event->buttons() & Qt::RightButton)) {
        QPointF last = mapFromScene(event->lastScenePos());
        QPointF now = mapFromScene(event->scenePos());
        QPointF delta = now-last;

        setPos(pos() + delta);

        prepareGeometryChange();
    }
    else if(event->buttons() & Qt::RightButton) {
        QPointF last = mapFromScene(event->lastScenePos());
        QPointF now = mapFromScene(event->scenePos());
        QPointF delta = now-last;

        m_size.setWidth(m_size.width() + delta.x());
        m_size.setHeight(m_size.height() + delta.y());
        prepareGeometryChange();
    }
}
