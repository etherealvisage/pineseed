#ifndef ConversationView_H
#define ConversationView_H

#include <QGraphicsView>

#include "Node.h"

class ConversationView : public QGraphicsView { Q_OBJECT
private:
    enum {
        DragMode,
        SelectMode,
        InsertMode
    } m_viewMode;
private:
    QGraphicsItem *m_origin;
public:
    ConversationView();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
public:
    void enterDragMode();
    void enterSelectMode();
    void enterInsertMode();
signals:
    void clicked(QPointF pos);
    void selected(ConversationObject *item);
private:
    ConversationObject *objectAt(QPoint uViewportPos);
};

#endif
