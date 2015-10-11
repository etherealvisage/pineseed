#ifndef ConversationView_H
#define ConversationView_H

#include <QGraphicsView>

class ConversationView : public QGraphicsView { Q_OBJECT
private:
    QPoint m_lastMousePos;
public:
    ConversationView();
protected:
    void mouseMoveEvent(QMouseEvent *event);
private slots:
    void addNode();
};

#endif
