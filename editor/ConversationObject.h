#ifndef ConversationObject_H
#define ConversationObject_H

#include <QGraphicsObject>

class QGraphicsView;

class ConversationObject : public QGraphicsObject {
public:
    virtual ~ConversationObject() {}

    virtual void contextMenu(QGraphicsView *view, QPoint pos) = 0;
    virtual void edit(QGraphicsView *view) = 0;
};

#endif
