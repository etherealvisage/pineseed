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

    const QString &label() const { return m_label; }

    QVector<Link *> &links() { return m_links; }
    const QVector<Link *> &links() const { return m_links; }

    QStandardItemModel *actionModel() const { return m_actionModel; }

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);

    virtual void edit(QFormLayout *layout);
    virtual bool isSelection(QPointF point);

    virtual void serialize(QXmlStreamWriter &xml,
        const QMap<ConversationObject *, int> &itemID);
    virtual void deserialize(QXmlStreamReader &xml,
        const QMap<int, ConversationObject *> &items);
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private:
    void actionSerializeHelper(QXmlStreamWriter &xml, 
        const QMap<ConversationObject *, int> &itemID, QStandardItem *action);
    QStandardItem *actionDeserializeHelper(QXmlStreamReader &xml, 
        const QMap<int, ConversationObject *> &items);
};

#endif
