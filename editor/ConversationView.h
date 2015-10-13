#ifndef ConversationView_H
#define ConversationView_H

#include <QGraphicsView>

#include "Node.h"

class ConversationView : public QGraphicsView { Q_OBJECT
private:
    enum {
        DragMode,
        SelectMode
    } m_viewMode;
private:
    QGraphicsItem *m_origin;
    QPoint m_lastMousePos;
public:
    ConversationView();
protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    void enterDragMode();
    void enterSelectMode();
signals:
    void selected(QGraphicsItem *item);
private slots:
    void addNode();
    void editNode();
    void removeNode();
    void addLink();
    void establishLink(QGraphicsItem *item);
    void changeLinkLabel();
    void removeLink();
};

#endif
