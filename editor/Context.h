#ifndef Context_H
#define Context_H

#include <QPointer>

#include "ConversationObject.h"

class ConversationContext;

class Context : public ConversationObject {
private:
    int m_id;
    QSizeF m_size;
    bool m_selected;
    QPointer<ConversationContext> m_context;
public:
    Context();
    virtual ~Context();
    
    virtual int id() const { return m_id; }

    virtual void select() { m_selected = true; update(); }
    virtual void deselect() { m_selected = false; update(); }

    QString label() const;

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *style, QWidget *widget);

    virtual void edit(ConversationDataInterface *interface,
        ConversationData *data, QFormLayout *layout);
    virtual bool isSelection(QPointF point);

    virtual void serialize(QXmlStreamWriter &xml);
    virtual void deserialize(QDomElement &xml,
        const QMap<int, ConversationObject *> &objs, ConversationData *data);
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif
