#include <cmath>

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QXmlStreamWriter>
#include <QFormLayout>
#include <QLabel>

#include "Platform.h"
#include "PlatformData.h"

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
    snappedPos.setX(snap(snappedPos.x()));
    snappedPos.setY(snap(snappedPos.y()));
    QSizeF snappedSize = m_size;
    snappedSize.setWidth(snap(m_size.width()));
    snappedSize.setHeight(snap(m_size.height()));

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

void Platform::edit(PlatformData *data, QFormLayout *layout) {
    if(m_id == -1) m_id = data->getAvailableID();

    layout->addRow(new QLabel(tr("Platform")));
}

void Platform::serialize(QXmlStreamWriter &xml) {
    xml.writeStartElement("platform");

    xml.writeAttribute("id", QString().setNum(m_id));
    xml.writeAttribute("x", QString().setNum(snap(pos().x())));
    xml.writeAttribute("y", QString().setNum(snap(pos().y())));
    xml.writeAttribute("width", QString().setNum(snap(m_size.width())));
    xml.writeAttribute("height", QString().setNum(snap(m_size.height())));

    xml.writeEndElement(); // </platform>
}

void Platform::deserialize(QDomElement &xml,
    const QMap<int, PlatformObject *> &objs, PlatformData *data) {

}

double Platform::snap(double v) {
    return std::floor(v/25.0)*25.0;
}
