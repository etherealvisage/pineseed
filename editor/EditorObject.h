#ifndef EditorObject_H
#define EditorObject_H

#include <QGraphicsObject>

class EditorObject : public QGraphicsObject { Q_OBJECT
public:
    virtual int id() const = 0;

    virtual void select() {}
    virtual void deselect() {}
    
    virtual bool isSelection(QPointF point) = 0;
signals:
    void changed();
};

#endif
