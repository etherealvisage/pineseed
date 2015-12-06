#ifndef PlatformData_H
#define PlatformData_H

#include <QSet>

class QXmlStreamWriter;
class QDomDocument;
class QWidget;

class PlatformData {
private:
    QSet<int> m_usedIDs;
public:
    PlatformData();

    QSet<int> &usedIDs() { return m_usedIDs; }

    int getAvailableID();

    void edit(QWidget *parent);

    void serialize(QXmlStreamWriter &xml);
    void deserialize(QDomDocument &doc);
};

#endif
