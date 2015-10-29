#ifndef Node_H
#define Node_H

#include <QGraphicsObject>

#include "ConversationObject.h"

class Action;
class Link;

class Node : public ConversationObject { Q_OBJECT
private:
    QSizeF m_size;
    QString m_label;
    //QVector<Action *> m_actions;
    QVector<Link *> m_links;
public:
    Node();
    virtual ~Node();

    //QVector<Action *> &actions() { return m_actions; }
    QVector<Link *> &links() { return m_links; }
    const QVector<Link *> &links() const { return m_links; }

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);

    virtual void edit(QFormLayout *layout);
    virtual bool isSelection(QPointF point);
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private slots:
};

#endif
