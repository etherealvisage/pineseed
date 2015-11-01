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
    bool m_selected;
    bool m_isEntry;
public:
    Node();
    virtual ~Node();

    virtual void select() { m_selected = true; update(); }
    virtual void deselect() { m_selected = false; update(); }

    const QString &label() const { return m_label; }

    QVector<Link *> &links() { return m_links; }
    const QVector<Link *> &links() const { return m_links; }

    QStandardItemModel *actionModel() const { return m_actionModel; }

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);

    virtual void edit(ConversationDataInterface *interface,
        ConversationData *data, QFormLayout *layout);
    virtual bool isSelection(QPointF point);

    virtual void serialize(QXmlStreamWriter &xml,
        const QMap<ConversationObject *, int> &itemID);
    virtual void deserialize(QDomElement &xml,
        const QMap<int, ConversationObject *> &objs);
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private:
};

#endif
