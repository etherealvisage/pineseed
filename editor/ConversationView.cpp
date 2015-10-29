#include <QAction>
#include <QInputDialog>
#include <QMouseEvent>
#include <QMenu>
#include <QScrollBar>

#include "ConversationView.h"
#include "moc_ConversationView.cpp"
#include "ConversationWindow.h" // for Mode

#include "Node.h"

ConversationView::ConversationView() : QGraphicsView(new QGraphicsScene()),
    m_viewMode(DragMode) {

    setMouseTracking(true);

    setSceneRect(-10000, -10000, 20000, 20000);

    setViewportUpdateMode(FullViewportUpdate);

    enterDragMode();
}

void ConversationView::mousePressEvent(QMouseEvent *event) {
    auto obj = objectAt(event->pos());
    if(obj) {
        if(event->button() == Qt::LeftButton && m_viewMode == SelectMode) {
            emit(selected(obj));
        }
    }
    else if(event->button() == Qt::LeftButton && m_viewMode == InsertMode) {
        emit(clicked(mapToScene(event->pos())));
    }
    else {
        QGraphicsView::mousePressEvent(event);
    }
}

void ConversationView::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);

    auto item = objectAt(event->pos());
    if(m_viewMode == SelectMode && item) {
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

void ConversationView::enterInsertMode() {
    m_viewMode = InsertMode;
    setCursor(Qt::CrossCursor);
    setDragMode(ScrollHandDrag);
}

#if 0
void ConversationView::addNode() {
    //Node *node = new Node();
    //scene()->addItem(node);

    //node->setPos(mapToScene(mapFromGlobal(m_lastMousePos)));
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

    //Link *link = new Link(origin, target);
    //origin->addLink(link);
    //target->addLink(link);

    //scene()->addItem(link);
}

void ConversationView::changeLinkLabel() {
    Link *link = dynamic_cast<Link *>(m_origin);
    bool ok = true;
    QString newLabel = QInputDialog::getText(this, tr("Link label"),
        tr("Enter new label name:"), QLineEdit::Normal, link->label(), &ok);
    if(ok && newLabel.length() > 0) {
        link->setLabel(newLabel);
        viewport()->update();
    }
}

void ConversationView::removeLink() {
    Link *link = dynamic_cast<Link *>(m_origin);
    link->from()->removeLink(link);
    link->to()->removeLink(link);
    scene()->removeItem(link);
}
#endif

ConversationObject *ConversationView::objectAt(QPoint uViewportPos) {
    auto items = this->items(uViewportPos);
    for(auto item : items) {
        auto obj = dynamic_cast<ConversationObject *>(item);
        if(!obj) break;

        auto pos = mapToScene(viewport()->mapFromParent(uViewportPos));
        
        if(!obj->isSelection(obj->mapFromScene(pos))) continue;
        return obj;
    }
    return nullptr;
}
