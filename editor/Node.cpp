#include <QDialog>
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

#include "Node.h"
#include "moc_Node.cpp"

#include "Action.h"
#include "Link.h"

Node::Node() {
    m_size = QSizeF(150, 100);

    setFlags(ItemIsSelectable);
}

QRectF Node::boundingRect() const {
    return QRectF(QPointF(0, 0), m_size);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *style,
    QWidget *widget) {

    QBrush b;
    b.setColor(QColor::fromRgb(240, 240, 240));
    b.setStyle(Qt::SolidPattern);
    painter->setBrush(b);
    painter->setBackgroundMode(Qt::OpaqueMode);
    painter->drawRect(QRectF(QPointF(0,0), m_size));
}

void Node::removeLink(Link *link) {
    int in = m_links.indexOf(link);
    if(in != -1) m_links.remove(in);
}

bool Node::hasLink(Node *to) {
    for(int i = 0; i < m_links.size(); i ++) {
        if(m_links[i]->to() == to) return true;
    }
    return false;
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

void Node::edit(QWidget *parent) {
    QDialog dialog(parent);

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

    connect(&actionsList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
        this, SLOT(editActionProxy(QListWidgetItem*)));

    for(auto action : m_actions) {
        auto i = new QListWidgetItem(action->description(), &actionsList);
        i->setData(Qt::UserRole,
            qVariantFromValue(reinterpret_cast<quint64>(action)));
    }

    QPushButton addActionButton(tr("&Add action"));
    grid.addWidget(&addActionButton, grid.rowCount(), 1);
    connect(&addActionButton, SIGNAL(clicked(bool)), this, SLOT(addAction()));
    QPushButton removeActionButton(tr("&Remove action"));
    grid.addWidget(&removeActionButton, grid.rowCount()-1, 2);
    connect(&removeActionButton, SIGNAL(clicked(bool)), this,
        SLOT(removeAction()));

    QLabel linksLabel(tr("Links:"));
    grid.addWidget(&linksLabel, grid.rowCount(), 0);
    QListWidget linksList;
    grid.addWidget(&linksList, grid.rowCount()-1, 1, 1, 2);
    linksList.setDragDropMode(QAbstractItemView::DragDrop);
    linksList.setDefaultDropAction(Qt::MoveAction);

    for(auto link : m_links) {
        auto i = new QListWidgetItem(link->label(), &linksList);
        i->setData(Qt::UserRole,
            qVariantFromValue(reinterpret_cast<quint64>(link)));
    }

    QPushButton okButton(QString("&Done"));
    okButton.setDefault(true);
    grid.addWidget(&okButton, grid.rowCount(), 0, 1, 3, Qt::AlignHCenter);
    connect(&okButton, SIGNAL(clicked(bool)), &dialog, SLOT(accept()));

    dialog.exec();

    m_label = labelText.text();

    m_links.clear();
    for(int r = 0; r < linksList.count(); r ++) {
        auto i = linksList.item(r);
        m_links.push_back(
            reinterpret_cast<Link *>(i->data(Qt::UserRole).value<quint64>()));
    }

    m_actions.clear();
    for(int r = 0; r < actionsList.count(); r ++) {
        auto i = actionsList.item(r);
        m_actions.push_back(
            reinterpret_cast<Action *>(i->data(Qt::UserRole).value<quint64>()));
    }
}

void Node::addAction() {
    Action::createAction(nullptr);
}

void Node::removeAction() {
    
}

void Node::editActionProxy(QListWidgetItem *item) {
    auto action =
        reinterpret_cast<Action *>(item->data(Qt::UserRole).value<quint64>());
    
    action->edit(item->listWidget());
}
