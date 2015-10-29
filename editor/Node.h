#ifndef Node_H
#define Node_H

#include <QGraphicsObject>

#include "ConversationObject.h"

//class Action;

class Node : public ConversationObject { Q_OBJECT
private:
    QSizeF m_size;
    QString m_label;
    //QVector<Action *> m_actions;
public:
    Node();
    virtual ~Node();

    //QVector<Action *> &actions() { return m_actions; }

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);

    virtual void edit(QFormLayout *layout);
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private slots:
};

#endif
