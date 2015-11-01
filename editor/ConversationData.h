#ifndef ConversationData_H
#define ConversationData_H

#include <QString>
#include <QList>
#include <QSet>

class QWidget;
class QXmlStreamWriter;
class QDomDocument;

class ConversationData {
private:
    QList<QString> m_characterNames;
    QSet<int> m_usedIDs;
public:
    QList<QString> &characterNames() { return m_characterNames; }

    QSet<int> &usedIDs() { return m_usedIDs; }

    int getAvailableID();

    void edit(QWidget *parent);

    void serialize(QXmlStreamWriter &xml);
    void deserialize(QDomDocument &doc);
};

#endif
