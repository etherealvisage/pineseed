#ifndef PlatformObject_H
#define PlatformObject_H

#include "EditorObject.h"

class QXmlStreamWriter;
class QDomElement;
class QFormLayout;

class PlatformData;

class PlatformObject : public EditorObject {
public:
    virtual void edit(PlatformData *data, QFormLayout *layout) = 0;

    virtual void serialize(QXmlStreamWriter &xml) = 0;
    virtual void deserialize(QDomElement &xml,
        const QMap<int, PlatformObject *> &objs,
        PlatformData *data) = 0;
};

#endif
