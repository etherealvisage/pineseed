#ifndef Node_H
#define Node_H

#include <QGraphicsObject>

#include "ConversationObject.h"

class Link;
class Action;


class Node : public ConversationObject { Q_OBJECT
private:
    QSizeF m_size;
    QString m_label;
    QVector<Link *> m_links;
    QVector<Action *> m_actions;
public:
    Node();
    virtual ~Node();

    QVector<Action *> &actions() { return m_actions; }

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);

    QVector<Link *> &links() { return m_links; }
    void addLink(Link *link) { m_links.push_back(link); }
    void removeLink(Link *link);
    bool hasLink(Node *to);

    virtual void edit(QGraphicsView *parent);
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif
