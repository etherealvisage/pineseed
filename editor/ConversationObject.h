#ifndef ConversationObject_H
#define ConversationObject_H

#include <QGraphicsObject>
#include <QMap>

class QGraphicsView;
class QFormLayout;

class QXmlStreamWriter;
class QXmlStreamReader;
class QDomElement;

class ConversationObject : public QGraphicsObject { Q_OBJECT
public:
    virtual ~ConversationObject() {}

    virtual void edit(QFormLayout *layout) = 0;
    virtual bool isSelection(QPointF point) = 0;

    virtual void serialize(QXmlStreamWriter &xml,
        const QMap<ConversationObject *, int> &itemID) = 0;
    virtual void deserialize(QDomElement &xml,
        const QMap<int, ConversationObject *> &objs) = 0;
signals:
    void changed();
};

#endif