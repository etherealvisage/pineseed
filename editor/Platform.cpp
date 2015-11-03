#include <QPainter>
#include <QGraphicsSceneMouseEvent>

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
    painter->drawRect(QRectF(QPointF(0,0), m_size));

    /*QFontMetricsF fm((QFont()));

    qreal width = fm.width(m_label);
    painter->setPen(Qt::black);
    painter->setBrush(Qt::NoBrush);
    painter->drawText(boundingRect().center() - QPointF(width/2,0), m_label);*/
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
