#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QFormLayout>

#include "Link.h"
#include "moc_Link.cpp"
#include "Node.h"

Link::Link(Node *from, Node *to) : m_from(from), m_to(to) {
    
}

Link::~Link() {
    
}

QRectF Link::boundingRect() const {
    QRectF result, from = m_from->boundingRect(), to = m_to->boundingRect();
    result.setLeft(qMin(from.left(), to.left()));
    result.setTop(qMin(from.top(), to.top()));
    result.setRight(qMax(from.right(), to.right()));
    result.setBottom(qMax(from.bottom(), to.bottom()));
    return result;
}

void Link::paint(QPainter *painter,
    const QStyleOptionGraphicsItem *style, QWidget *widget) {

}


void Link::edit(QFormLayout *layout) {
    QLineEdit *edit = new QLineEdit();
    layout->addRow(tr("Label:"), edit);
    connect(edit, SIGNAL(textChanged(const QString &)),
        this, SLOT(labelUpdated(const QString &)));
}

void Link::labelUpdated(const QString &newLabel) {
    //m_label = newLabel;
}
