#include <cmath>

#include <QGraphicsSceneMouseEvent>
#include <QXmlStreamWriter>
#include <QDomElement>
#include <QPainter>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>

#include "Context.h"
#include "ConversationContext.h"
#include "ConversationData.h"
#include "ConversationDataInterface.h"
#include "Node.h"

Context::Context() {
    m_id = -1;
    m_selected = false;
    m_context = nullptr;
    m_size = QSizeF(100,100);

    setFlags(ItemIsSelectable | ItemIsMovable);
}

Context::~Context() {

}

QString Context::label() const {
    if(m_context.isNull()) return "<invalid context>";
    return m_context->label();
}

QRectF Context::boundingRect() const {
    return QRectF(QPointF(0,0), m_size);
}

void Context::paint(QPainter *painter,
    const QStyleOptionGraphicsItem *style, QWidget *widget) {

    QBrush b;
    if(m_context.isNull()) {
        b.setColor(qRgb(0, 0, 0));
        b.setStyle(Qt::SolidPattern);
    }
    else {
        b = m_context->deriveBrush();
    }

    painter->setBrush(b);
    painter->drawEllipse(QRectF(QPointF(0,0), m_size));
    painter->setBrush(Qt::red);
    if(m_selected) painter->drawEllipse(
        QRectF(QPointF(m_size.width()/4,m_size.height()/4), m_size/2));
}

void Context::edit(ConversationDataInterface *interface,
    ConversationData *data, QFormLayout *layout) {

    if(m_id == -1) m_id = data->getAvailableID();

    auto nameLabel = new QLabel(tr("Not selected"));
    layout->addRow(new QLabel("Current context:"), nameLabel);
    auto chooseButton = new QPushButton(tr("Change"));
    layout->addRow(nullptr, chooseButton);

    if(m_context) nameLabel->setText(m_context->label());

    connect(chooseButton, &QPushButton::clicked,
        [=]() {
            m_context = data->selectContext(chooseButton);
            if(m_context) nameLabel->setText(m_context->label());
            else nameLabel->setText("Not selected");
            update();
        });

    auto paintButton = new QPushButton(tr("&Paint context"));
    layout->addRow(paintButton);

    connect(paintButton, &QPushButton::clicked,
        [=]() {
            if(m_context.isNull()) return;
            interface->selectObjects(
                std::function<bool (ConversationObject *)>(
                    [=](ConversationObject *object) {
                        return dynamic_cast<Node *>(object);
                    }),
                std::function<bool (ConversationObject *)>(
                    [=](ConversationObject *object) {
                        dynamic_cast<Node *>(object)->setContext(m_context);
                        return false;
                    }));
        });
}

bool Context::isSelection(QPointF point) {
    point -= boundingRect().center();
    return std::sqrt(point.x()*point.x() + point.y()*point.y())
        <= m_size.width()/2;
}

void Context::serialize(QXmlStreamWriter &xml) {
    xml.writeStartElement("context-ref");

    xml.writeAttribute("id", QString().setNum(m_id));
    xml.writeAttribute("x", QString().setNum(pos().x()));
    xml.writeAttribute("y", QString().setNum(pos().y()));
    xml.writeAttribute("width", QString().setNum(m_size.width()));
    xml.writeAttribute("height", QString().setNum(m_size.height()));
    if(!m_context.isNull())
        xml.writeAttribute("context", QString().setNum(m_context->id()));

    xml.writeEndElement(); // </context-ref>
}

void Context::deserialize(QDomElement &xml,
    const QMap<int, ConversationObject *> &objs, ConversationData *data) {

    m_id = xml.attribute("id").toInt();
    setPos(QPointF(xml.attribute("x").toDouble(),
        xml.attribute("y").toDouble()));
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
