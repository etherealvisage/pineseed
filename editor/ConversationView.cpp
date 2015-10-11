#include <QAction>
#include <QMouseEvent>

#include "ConversationView.h"
#include "moc_ConversationView.cpp"

#include "Node.h"

ConversationView::ConversationView() : QGraphicsView(new QGraphicsScene()) {
    QAction *contextAddNode = new QAction(tr("&Add node"), this);
    connect(contextAddNode, SIGNAL(triggered(bool)), this, SLOT(addNode()));
    addAction(contextAddNode);

    setContextMenuPolicy(Qt::ActionsContextMenu);
    setMouseTracking(true);
}

void ConversationView::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);
    m_lastMousePos = event->globalPos();
}

void ConversationView::addNode() {
    qDebug("Adding node...");

    qDebug("Global position: (%d, %d)", m_lastMousePos.x(),
        m_lastMousePos.y());

    auto p = mapFromGlobal(m_lastMousePos);
    qDebug("Local position: (%d, %d)", p.x(), p.y());

    Node *node = new Node();
    scene()->addItem(node);

    node->setPos(mapFromGlobal(m_lastMousePos));
    centerOn(node);
}
