#ifndef Node_H
#define Node_H

#include <QGraphicsObject>

#include "ConversationObject.h"

class QTreeView;
class QStandardItem;
class QStandardItemModel;

class Action;
class Link;

class Node : public ConversationObject { Q_OBJECT
public:
private:
    QSizeF m_size;
    QString m_label;
    QVector<Link *> m_links;
    QStandardItemModel *m_actionModel;
public:
    Node();
    virtual ~Node();

    QVector<Link *> &links() { return m_links; }
    const QVector<Link *> &links() const { return m_links; }

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);

    virtual void edit(QFormLayout *layout);
    virtual bool isSelection(QPointF point);
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private:
};

#endif
