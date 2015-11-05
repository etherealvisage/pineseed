#include <cmath>

#include <QGraphicsSceneMouseEvent>
#include <QXmlStreamWriter>
#include <QDomElement>
#include <QPainter>

#include "Context.h"
#include "ConversationContext.h"
#include "ConversationData.h"

Context::Context() {
    m_id = -1;
    m_selected = false;
    m_context = nullptr;
}

Context::~Context() {

}

QString Context::label() const {
    if(m_context.isNull()) return "<invalid context>";
    return m_context->label();
}

QRectF Context::boundingRect() const {
    return QRectF(QPointF(-m_size.width()/2, -m_size.height()/2), m_size);
}

void Context::paint(QPainter *painter,
    const QStyleOptionGraphicsItem *style, QWidget *widget) {

    painter->drawEllipse(QRectF(pos(), m_size));
}

void Context::edit(ConversationDataInterface *interface,
    ConversationData *data, QFormLayout *layout) {

}

bool Context::isSelection(QPointF point) {
    point -= pos();
    return std::sqrt(point.x()*point.x() + point.y()+point.y())
        <= m_size.width()/2;
}

void Context::serialize(QXmlStreamWriter &xml) {
    xml.writeStartElement("context-ref");

    xml.writeAttribute("id", QString().setNum(m_id));
    xml.writeAttribute("x", QString().setNum(pos().x()));
    xml.writeAttribute("y", QString().setNum(pos().x()));
    xml.writeAttribute("width", QString().setNum(m_size.width()/2));
    if(!m_context.isNull())
        xml.writeAttribute("context", QString().setNum(m_context->id()));

    xml.writeEndElement(); // </context-ref>
}

void Context::deserialize(QDomElement &xml,
    const QMap<int, ConversationObject *> &objs, ConversationData *data) {

    m_id = xml.attribute("id").toInt();
    pos().setX(xml.attribute("x").toDouble());
    pos().setY(xml.attribute("y").toDouble());
    m_size.setWidth(xml.attribute("width").toDouble());
    m_size.setHeight(xml.attribute("height").toDouble());
    bool ok = false;
    int cid = xml.attribute("context").toInt(&ok);
    if(ok) m_context = data->getContextByID(cid);
}

void Context::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if(event->buttons() & Qt::RightButton) {
        QPointF last = mapFromScene(event->lastScenePos());
        QPointF now = mapFromScene(event->scenePos());
        QPointF delta = now-last;

        setPos(pos() + delta);

        prepareGeometryChange();
    }
}
