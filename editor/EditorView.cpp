#include <QAction>
#include <QInputDialog>
#include <QMouseEvent>
#include <QMenu>
#include <QScrollBar>

#include "EditorView.h"
#include "moc_EditorView.cpp"
#include "ConversationWindow.h" // for Mode

#include "Node.h"

EditorView::EditorView() : QGraphicsView(new QGraphicsScene()),
    m_viewMode(DragMode) {

    setMouseTracking(true);

    setSceneRect(-10000, -10000, 20000, 20000);

    setViewportUpdateMode(FullViewportUpdate);

    enterDragMode();
}

void EditorView::mousePressEvent(QMouseEvent *event) {
    auto obj = objectAt(event->pos());
    if(obj) {
        if(event->button() == Qt::LeftButton && m_viewMode == SelectMode) {
            emit(selected(obj));
        }
        QGraphicsView::mousePressEvent(event);
    }
    else if(event->button() == Qt::LeftButton && m_viewMode == InsertMode) {
        emit(clicked(mapToScene(event->pos())));
    }
    else {
        QGraphicsView::mousePressEvent(event);
    }
}

void EditorView::mouseMoveEvent(QMouseEvent *event) {
    auto item = objectAt(event->pos());
    if(m_viewMode == SelectMode && item) {
        setCursor(Qt::CrossCursor);
        setDragMode(NoDrag);
    }
    else {
        setCursor(Qt::OpenHandCursor);
        setDragMode(ScrollHandDrag);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void EditorView::enterDragMode() {
    m_viewMode = DragMode;
    setCursor(Qt::OpenHandCursor);
    setDragMode(ScrollHandDrag);
}

void EditorView::enterSelectMode() {
    m_viewMode = SelectMode;
    setCursor(Qt::CrossCursor);
    setDragMode(ScrollHandDrag);
}

void EditorView::enterInsertMode() {
    m_viewMode = InsertMode;
    setCursor(Qt::CrossCursor);
    setDragMode(ScrollHandDrag);
}

EditorObject *EditorView::objectAt(QPoint uViewportPos) {
    auto items = this->items(uViewportPos);
    for(auto item : items) {
        auto obj = dynamic_cast<EditorObject *>(item);
        if(!obj) break;

        auto pos = mapToScene(viewport()->mapFromParent(uViewportPos));
        
        if(!obj->isSelection(obj->mapFromScene(pos))) continue;
        return obj;
    }
    return nullptr;
}
