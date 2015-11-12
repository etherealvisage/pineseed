#ifndef Node_H
#define Node_H

#include <functional>

#include <QGraphicsObject>

#include "LinkableObject.h"

class QTreeView;
class QStandardItem;
class QStandardItemModel;

class Action;
class Link;
class ConversationContext;

class Node : public LinkableObject { Q_OBJECT
private:
    int m_id;
    QSizeF m_size;
    QString m_label;
    QStandardItemModel *m_actionModel;
    bool m_selected;
    bool m_isEntry;
    QPointer<ConversationContext> m_context;
public:
    Node();
    virtual ~Node();

    virtual int id() const { return m_id; }

    virtual void select() { m_selected = true; update(); }
    virtual void deselect() { m_selected = false; update(); }

    const QString &label() const { return m_label; }

    QStandardItemModel *actionModel() const { return m_actionModel; }

    QPointer<ConversationContext> context() const { return m_context; }
    void setContext(QPointer<ConversationContext> context)
        { m_context = context; }

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);

    virtual void edit(ConversationDataInterface *interface,
        ConversationData *data, QFormLayout *layout);
    virtual bool isSelection(QPointF point);

    virtual void serialize(QXmlStreamWriter &xml);
    virtual void deserialize(QDomElement &xml,
        const QMap<int, ConversationObject *> &objs, ConversationData *data);

    void visitActions(std::function<void (QStandardItem *)> visitor);
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private:
};

#endif
