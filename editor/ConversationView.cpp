#include <QAction>
#include <QInputDialog>
#include <QMouseEvent>
#include <QMenu>

#include "ConversationView.h"
#include "moc_ConversationView.cpp"

#include "Node.h"
#include "Link.h"

ConversationView::ConversationView() : QGraphicsView(new QGraphicsScene()) {
    setMouseTracking(true);

    setSceneRect(0, 0, 1, 1);

    Node *node = new Node();
    scene()->addItem(node);

    m_selectMode = false;

    setViewportUpdateMode(FullViewportUpdate);
}

void ConversationView::mousePressEvent(QMouseEvent *event) {
    auto item = itemAt(event->pos());
    if(event->button() == Qt::LeftButton && m_selectMode) {
        m_selectMode = false;
        setCursor(Qt::ArrowCursor);

        if(item) emit(selected(item));
    }
    else {
        QGraphicsView::mousePressEvent(event);
    }
}

void ConversationView::contextMenuEvent(QContextMenuEvent *event) {
    auto item = itemAt(event->pos());
    m_origin = item;
    if(item) {
        if(dynamic_cast<Node *>(item)) {
            QMenu menu;

            QAction *linkAction = new QAction(tr("Add &Link"), &menu);
            menu.addAction(linkAction);
            connect(linkAction, SIGNAL(triggered(bool)), this, SLOT(addLink()));

            menu.exec(mapToGlobal(event->pos()));
        }
        else if(dynamic_cast<Link *>(item)) {
            QMenu menu;

            QAction *contextChangeLabel = new QAction(tr("&Change label"), this);
            connect(contextChangeLabel, SIGNAL(triggered(bool)), this,
                SLOT(changeLinkLabel()));
            menu.addAction(contextChangeLabel);
            QAction *contextRemoveLink = new QAction(tr("&Remove link"), this);
            menu.addAction(contextRemoveLink);

            menu.exec(mapToGlobal(event->pos()));
        }
    }
    else {
        QMenu menu;

        QAction *contextAddNode = new QAction(tr("&Add node"), this);
        connect(contextAddNode, SIGNAL(triggered(bool)), this,
            SLOT(addNode()));
        menu.addAction(contextAddNode);

        menu.exec(mapToGlobal(event->pos()));
    }
}

void ConversationView::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);
    m_lastMousePos = event->globalPos();
}

void ConversationView::enterSelectMode() {
    m_selectMode = true;
    setCursor(Qt::CrossCursor);
}

void ConversationView::addNode() {
    Node *node = new Node();
    scene()->addItem(node);

    node->setPos(mapToScene(mapFromGlobal(m_lastMousePos)));
    centerOn(node);
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

    Link *link = new Link(origin, target);

    scene()->addItem(link);
}

void ConversationView::changeLinkLabel() {
    Link *link = dynamic_cast<Link *>(m_origin);
    bool ok = true;
    QString newLabel = QInputDialog::getText(this, tr("Link label"),
        tr("Enter new label name:"), QLineEdit::Normal, link->label(), &ok);
    if(ok) {
        link->setLabel(newLabel);
    }
}
