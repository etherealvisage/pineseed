#ifndef ConversationData_H
#define ConversationData_H

#include <QString>
#include <QList>

class QWidget;
class QXmlStreamWriter;
class QDomDocument;

class ConversationData {
private:
    QList<QString> m_characterNames;
public:
    QList<QString> &characterNames() { return m_characterNames; }

    void edit(QWidget *parent);

    void serialize(QXmlStreamWriter &xml);
    void deserialize(QDomDocument &doc);
};

#endif
