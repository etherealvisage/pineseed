#ifndef ConversationView_H
#define ConversationView_H

#include <QGraphicsView>

#include "Node.h"

class ConversationView : public QGraphicsView { Q_OBJECT
private:
    QGraphicsItem *m_origin;
    QPoint m_lastMousePos;
    bool m_selectMode;
public:
    ConversationView();
protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    void enterSelectMode();
signals:
    void selected(QGraphicsItem *item);
private slots:
    void addNode();
    void addLink();
    void establishLink(QGraphicsItem *item);
};

#endif