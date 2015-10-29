#ifndef ConversationObject_H
#define ConversationObject_H

#include <QGraphicsObject>

class QGraphicsView;
class QFormLayout;

class ConversationObject : public QGraphicsObject { Q_OBJECT
public:
    virtual ~ConversationObject() {}

    virtual void edit(QFormLayout *layout) = 0;
signals:
    void changed();
};

#endif
