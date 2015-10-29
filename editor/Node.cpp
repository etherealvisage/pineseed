#include <QDialog>
#include <QFont>
#include <QFormLayout>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsView>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QPainter>
#include <QPushButton>
#include <QStandardItemModel>
#include <QTreeView>
#include <QStackedWidget>
#include <QComboBox>

#include "Node.h"
#include "moc_Node.cpp"

#include "Link.h"
#include "ActionEditor.h"

Node::Node() {
    m_size = QSizeF(150, 100);

    setFlags(ItemIsSelectable | ItemIsMovable);

    m_actionModel = new QStandardItemModel();
}

Node::~Node() {
    for(auto link : m_links) delete link;
}

QRectF Node::boundingRect() const {
    // accommodate borders
    const qreal border = 1.0;
    return QRectF(QPointF(0,0), m_size + QSizeF(border*2, border*2));
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *style,
    QWidget *widget) {

    QBrush b;
    b.setColor(QColor::fromRgb(240, 240, 240));
    b.setStyle(Qt::SolidPattern);
    painter->setBrush(b);
    painter->drawRect(QRectF(QPointF(0,0), m_size));

    QFontMetricsF fm((QFont()));

    qreal width = fm.width(m_label);
    painter->setPen(Qt::black);
    painter->setBrush(Qt::NoBrush);
    painter->drawText(boundingRect().center() - QPointF(width/2,0), m_label);
}

void Node::edit(QFormLayout *layout) {
    QLineEdit *labelEdit = new QLineEdit(m_label);
    layout->addRow(tr("Label:"), labelEdit);
    connect(labelEdit, &QLineEdit::textChanged,
        [=](const QString &label){ m_label = label; emit changed(); });

    ActionEditor *editor = new ActionEditor(m_actionModel);
    layout->addRow(tr(""), editor);

}

bool Node::isSelection(QPointF point) {
    return boundingRect().contains(point);
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton) {
        QPointF last = mapFromScene(event->lastScenePos());
        QPointF now = mapFromScene(event->scenePos());
        QPointF delta = now-last;

        setPos(pos() + delta);

        for(auto view : scene()->views()) {
            view->repaint();
        }
    }
}

