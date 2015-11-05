#ifndef ConversationObject_H
#define ConversationObject_H

#include <QGraphicsObject>
#include <QMap>

#include "EditorObject.h"

class QFormLayout;
class QXmlStreamWriter;
class QDomElement;

class ConversationData;
class ConversationDataInterface;

class ConversationObject : public EditorObject {
public:
    virtual ~ConversationObject() {}

    virtual void edit(ConversationDataInterface *interface,
        ConversationData *data, QFormLayout *layout) = 0;

    virtual void serialize(QXmlStreamWriter &xml) = 0;
    virtual void deserialize(QDomElement &xml,
        const QMap<int, ConversationObject *> &objs,
        ConversationData *data) = 0;
};

#endif
