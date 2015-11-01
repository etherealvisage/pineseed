#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QFormLayout>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QXmlStreamWriter>
#include <QDomElement>

#include "Link.h"
#include "moc_Link.cpp"
#include "Node.h"
#include "ConversationData.h"

Link::Link(Node *from, Node *to) : m_from(from), m_to(to), m_label("Label") {
    m_id = -1;
    if(from) m_from->links().push_back(this);
    if(to) m_to->links().push_back(this);

    this->setZValue(-1);
    m_selected = false;
}

Link::~Link() {
    m_from->links().remove(m_from->links().indexOf(this));
    m_to->links().remove(m_to->links().indexOf(this));
}

QRectF Link::boundingRect() const {
    QRectF result, from = m_from->boundingRect(), to = m_to->boundingRect();
    from.moveTopLeft(m_from->pos());
    to.moveTopLeft(m_to->pos());
    result.setLeft(qMin(from.left(), to.left()));
    result.setTop(qMin(from.top(), to.top()));
    result.setRight(qMax(from.right(), to.right()));
    result.setBottom(qMax(from.bottom(), to.bottom()));
    return result;
}

void Link::paint(QPainter *painter,
    const QStyleOptionGraphicsItem *style, QWidget *widget) {

    if(m_selected) painter->setPen(QPen(QBrush(Qt::green), 3.0));
    else painter->setPen(QPen(QBrush(Qt::darkGreen), 3.0));

    QLineF line(m_from->pos() + m_from->boundingRect().center(),
        m_to->pos() + m_to->boundingRect().center());
    painter->drawLine(line);

    painter->setPen(QPen(QBrush(Qt::blue), 5.0));
    QPointF line13 = (line.p1()*2+line.p2())/3;
    QPointF line23 = (line.p1()+line.p2()*2)/3;
    auto arrow1 = QLineF::fromPolar(15.0, line.angle() - 135);
    auto arrow2 = QLineF::fromPolar(15.0, line.angle() + 135);
    painter->drawLine(arrow1.translated(line13));
    painter->drawLine(arrow2.translated(line13));
    painter->drawLine(arrow1.translated(line23));
    painter->drawLine(arrow2.translated(line23));

    auto tbr = labelBoundingRect();

    painter->setPen(Qt::lightGray);
    if(m_selected) painter->setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
    else painter->setBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
    painter->drawRect(tbr);

    painter->setPen(QPen(QBrush(Qt::red), 3.0));
    painter->drawText(tbr.center() - QPointF(tbr.width()/2, -4), m_label);
}

void Link::edit(ConversationDataInterface *interface,
    ConversationData *data, QFormLayout *layout) {

    if(m_id == -1) {
        m_id = data->getAvailableID();
    }

    QLineEdit *edit = new QLineEdit(m_label);
    layout->addRow(tr("Label:"), edit);
    edit->setFocus();
    connect(edit, &QLineEdit::textChanged,
        [=](const QString &label){ m_label = label; emit changed(); });
}

bool Link::isSelection(QPointF point) {
    return labelBoundingRect().contains(point);
}

void Link::serialize(QXmlStreamWriter &xml) {
    xml.writeStartElement("link");

    xml.writeAttribute("id", QString().setNum(m_id));
    xml.writeAttribute("label", m_label);
    xml.writeAttribute("from", QString().setNum(m_from->id()));
    xml.writeAttribute("to", QString().setNum(m_to->id()));

    xml.writeEndElement();
}

void Link::deserialize(QDomElement &xml,
    const QMap<int, ConversationObject *> &objs) {

    m_id = xml.attribute("id").toInt();
    m_label = xml.attribute("label");
    m_from = dynamic_cast<Node *>(objs[xml.attribute("from").toInt()]);
    m_to = dynamic_cast<Node *>(objs[xml.attribute("to").toInt()]);

    m_from->links().push_back(this);
    m_to->links().push_back(this);

    prepareGeometryChange();
}

QRectF Link::labelBoundingRect() const {
    QPointF centre = ((m_from->pos() + m_from->boundingRect().center())
        + (m_to->pos() + m_to->boundingRect().center())) / 2.0;

    QFontMetricsF fm((QFont()));
    QRectF tbr = fm.boundingRect(m_label);
    if(m_label == "") tbr.setSize(QSizeF(10,10));
    tbr.moveCenter(centre);

    return tbr;
}
