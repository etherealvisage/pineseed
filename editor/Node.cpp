#include <QDialog>
#include <QFont>
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
#include <QFormLayout>

#include "Node.h"
#include "moc_Node.cpp"

#include "Link.h"

Node::Node() {
    m_size = QSizeF(150, 100);

    setFlags(ItemIsSelectable | ItemIsMovable);
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

void Node::edit(QFormLayout *layout) {
    QLineEdit *labelEdit = new QLineEdit(m_label);
    layout->addRow(tr("Label:"), labelEdit);
    connect(labelEdit, &QLineEdit::textChanged,
        [=](const QString &label){ m_label = label; emit changed(); });
#if 0
    QDialog dialog(widget);

    QGridLayout grid;
    dialog.setLayout(&grid);

    QLabel labelLabel(tr("Label:"));
    grid.addWidget(&labelLabel, grid.rowCount(), 0);
    QLineEdit labelText(m_label);
    grid.addWidget(&labelText, grid.rowCount()-1, 1, 1, 2);

    QLabel actionsLabel(tr("Actions:"));
    grid.addWidget(&actionsLabel, grid.rowCount(), 0);
    QListWidget actionsList;
    grid.addWidget(&actionsList, grid.rowCount()-1, 1, 1, 2);
    actionsList.setDragDropMode(QAbstractItemView::DragDrop);
    actionsList.setDefaultDropAction(Qt::MoveAction);

    connect(&actionsList, &QListWidget::itemDoubleClicked,
        [=](QListWidgetItem *item) {
            auto action = reinterpret_cast<Action *>(
                item->data(Qt::UserRole).value<quint64>());
            action->edit(parent);
        });

    for(auto action : m_actions) {
        auto i = new QListWidgetItem(action->description(), &actionsList);
        i->setData(Qt::UserRole,
            qVariantFromValue(reinterpret_cast<quint64>(action)));
    }

    QPushButton addActionButton(tr("&Add action"));
    grid.addWidget(&addActionButton, grid.rowCount(), 1);
    connect(&addActionButton, &QPushButton::clicked,
        [=,&actionsList](){
            auto action = Action::createAction(parent);
            if(!action) return;
            auto i = new QListWidgetItem(action->description(), &actionsList);
            i->setData(Qt::UserRole,
                qVariantFromValue(reinterpret_cast<quint64>(action)));
        });
    QPushButton removeActionButton(tr("&Remove action"));
    grid.addWidget(&removeActionButton, grid.rowCount()-1, 2);
    connect(&removeActionButton, &QPushButton::clicked,
        [=,&actionsList](){
            auto i = actionsList.currentItem();
            auto action = reinterpret_cast<Action *>(i->data(Qt::UserRole).value<quint64>());
            delete action;
            delete i;
        });

    QPushButton okButton(QString("&Done"));
    okButton.setDefault(true);
    grid.addWidget(&okButton, grid.rowCount(), 0, 1, 3, Qt::AlignHCenter);
    connect(&okButton, SIGNAL(clicked(bool)), &dialog, SLOT(accept()));

    dialog.exec();

    m_label = labelText.text();

    m_actions.clear();
    for(int r = 0; r < actionsList.count(); r ++) {
        auto i = actionsList.item(r);
        m_actions.push_back(
            reinterpret_cast<Action *>(i->data(Qt::UserRole).value<quint64>()));
    }

    // NOTE: dangerous...
    dynamic_cast<QGraphicsView *>(parent)->viewport()->update();
#endif
}

bool Node::isSelection(QPointF point) {
    return boundingRect().contains(point);
}
