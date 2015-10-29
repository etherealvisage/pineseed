#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QFormLayout>
#include <QPainter>
#include <QPen>
#include <QBrush>

#include "Link.h"
#include "moc_Link.cpp"
#include "Node.h"

Link::Link(Node *from, Node *to) : m_from(from), m_to(to) {
    m_label = "Label";
    m_from->links().push_back(this);
    m_to->links().push_back(this);
}

Link::~Link() {
    m_from->links().removeAll(this);
    m_to->links().removeAll(this);
}

QRectF Link::boundingRect() const {
    QRectF result, from = m_from->boundingRect(), to = m_to->boundingRect();
    from.setTopLeft(m_from->pos());
    to.setTopLeft(m_to->pos());
    result.setLeft(qMin(from.left(), to.left()));
    result.setTop(qMin(from.top(), to.top()));
    result.setRight(qMax(from.right(), to.right()));
    result.setBottom(qMax(from.bottom(), to.bottom()));
    return result;
}

void Link::paint(QPainter *painter,
    const QStyleOptionGraphicsItem *style, QWidget *widget) {

    painter->setPen(QPen(QBrush(Qt::darkGreen), 3.0));

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
    painter->setBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
    painter->drawRect(tbr);

    painter->setPen(QPen(QBrush(Qt::red), 3.0));
    painter->drawText(tbr.center() - QPointF(tbr.width()/2, -4), m_label);
}

void Link::edit(QFormLayout *layout) {
    QLineEdit *edit = new QLineEdit(m_label);
    layout->addRow(tr("Label:"), edit);
    connect(edit, &QLineEdit::textChanged,
        [=](const QString &label){ m_label = label; emit changed(); });
}

bool Link::isSelection(QPointF point) {
    return labelBoundingRect().contains(point);
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
