#include <QAction>
#include <QInputDialog>
#include <QMouseEvent>
#include <QMenu>

#include "ConversationView.h"
#include "moc_ConversationView.cpp"

#include "Node.h"
#include "Link.h"

ConversationView::ConversationView() : QGraphicsView(new QGraphicsScene()),
    m_viewMode(DragMode) {

    setMouseTracking(true);

    setSceneRect(-10000, -10000, 20000, 20000);

    Node *node = new Node();
    scene()->addItem(node);

    setViewportUpdateMode(FullViewportUpdate);

    enterDragMode();
}

void ConversationView::contextMenuEvent(QContextMenuEvent *event) {
    auto item = itemAt(event->pos());
    m_origin = item;
    bool found = false;
    if(item) {
        auto node = dynamic_cast<Node *>(item);
        if(node) {
            QMenu menu;

            QAction *editAction = new QAction(tr("&Edit"), &menu);
            menu.addAction(editAction);
            connect(editAction, SIGNAL(triggered(bool)), this, SLOT(editNode()));

            QAction *linkAction = new QAction(tr("Add &Link"), &menu);
            menu.addAction(linkAction);
            connect(linkAction, SIGNAL(triggered(bool)), this, SLOT(addLink()));

            QAction *removeAction = new QAction(tr("Remove &Node"), &menu);
            menu.addAction(removeAction);
            connect(removeAction, SIGNAL(triggered(bool)), this, SLOT(removeNode()));

            menu.exec(mapToGlobal(event->pos()));
            return;
        }
        auto link = dynamic_cast<Link *>(item);
        if(link && link->labelBoundingRect().contains(
            mapToScene(event->pos()))) {

            QMenu menu;

            QAction *contextChangeLabel = new QAction(tr("&Change label"), this);
            connect(contextChangeLabel, SIGNAL(triggered(bool)), this,
                SLOT(changeLinkLabel()));
            menu.addAction(contextChangeLabel);
            QAction *contextRemoveLink = new QAction(tr("&Remove link"), this);
            connect(contextRemoveLink, SIGNAL(triggered(bool)), this,
                SLOT(removeLink()));
            menu.addAction(contextRemoveLink);

            menu.exec(mapToGlobal(event->pos()));
            return;
        }
    }
    if(!found) {
        QMenu menu;

        QAction *contextAddNode = new QAction(tr("&Add node"), this);
        connect(contextAddNode, SIGNAL(triggered(bool)), this,
            SLOT(addNode()));
        menu.addAction(contextAddNode);

        menu.exec(mapToGlobal(event->pos()));
    }
}

void ConversationView::mouseDoubleClickEvent(QMouseEvent *event) {
    auto item = itemAt(event->pos());
    if(!item || m_viewMode != DragMode) return;

    auto link = dynamic_cast<Link *>(item);
    if(link && link->labelBoundingRect().contains(mapToScene(event->pos()))) {
        m_origin = item;
        changeLinkLabel();
    }
    auto node = dynamic_cast<Node *>(item);
    if(node) {
        node->edit(this);
    }

    QGraphicsView::mouseDoubleClickEvent(event);
}

void ConversationView::mousePressEvent(QMouseEvent *event) {
    auto item = itemAt(event->pos());
    auto link = dynamic_cast<Link *>(item);
    if(item && event->button() == Qt::LeftButton && m_viewMode == SelectMode
        && (!link || link->labelBoundingRect().contains(
            mapToScene(event->pos())))) {

        qDebug("selected");
        emit(selected(item));
        enterDragMode();
    }
    else {
        QGraphicsView::mousePressEvent(event);
    }
}


void ConversationView::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);
    m_lastMousePos = event->globalPos();

    auto item = itemAt(event->pos());
    if(m_viewMode == SelectMode && dynamic_cast<Node *>(item)) {
        setCursor(Qt::CrossCursor);
        setDragMode(NoDrag);
    }
    else {
        setCursor(Qt::OpenHandCursor);
        setDragMode(ScrollHandDrag);
    }
}

void ConversationView::enterDragMode() {
    m_viewMode = DragMode;
    setCursor(Qt::OpenHandCursor);
    setDragMode(ScrollHandDrag);
}

void ConversationView::enterSelectMode() {
    m_viewMode = SelectMode;
    setCursor(Qt::CrossCursor);
    setDragMode(ScrollHandDrag);
}

void ConversationView::addNode() {
    Node *node = new Node();
    scene()->addItem(node);

    node->setPos(mapToScene(mapFromGlobal(m_lastMousePos)));
}

void ConversationView::editNode() {
    auto node = dynamic_cast<Node *>(m_origin);
    node->edit(this);
}

void ConversationView::removeNode() {
    auto node = dynamic_cast<Node *>(m_origin);
    delete node;
    m_origin = nullptr;
}

void ConversationView::addLink() {
    enterSelectMode();
    connect(this, SIGNAL(selected(QGraphicsItem*)), this,
        SLOT(establishLink(QGraphicsItem*)));
}

void ConversationView::establishLink(QGraphicsItem *item) {
    disconnect(this, SIGNAL(selected(QGraphicsItem*)), 0, 0);

    Node *origin = dynamic_cast<Node *>(m_origin);
    Node *target = dynamic_cast<Node *>(item);

    if(!origin || !target) return;

    if(origin == target) return; // no self-links!
    if(origin->hasLink(target)) return; // no duplicate links!

    Link *link = new Link(origin, target);
    origin->addLink(link);
    target->addLink(link);

    scene()->addItem(link);
}

void ConversationView::changeLinkLabel() {
    Link *link = dynamic_cast<Link *>(m_origin);
    bool ok = true;
    QString newLabel = QInputDialog::getText(this, tr("Link label"),
        tr("Enter new label name:"), QLineEdit::Normal, link->label(), &ok);
    if(ok && newLabel.length() > 0) {
        link->setLabel(newLabel);
    }
}

void ConversationView::removeLink() {
    Link *link = dynamic_cast<Link *>(m_origin);
    link->from()->removeLink(link);
    link->to()->removeLink(link);
    scene()->removeItem(link);
}
