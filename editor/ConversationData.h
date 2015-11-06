#ifndef ConversationData_H
#define ConversationData_H

#include <QString>
#include <QList>
#include <QSet>
#include <QPointer>
#include <QMap>

class QWidget;
class QXmlStreamWriter;
class QDomDocument;
class QStandardItem;
class QStandardItemModel;

class ConversationContext;

class ConversationData {
private:
    const int IDData = 0x100;
private:
    QList<QString> m_characterNames;
    QSet<int> m_usedIDs;
    ConversationContext *m_rootContext;
    QMap<int, ConversationContext *> m_contexts;
public:
    ConversationData() : m_rootContext(nullptr) {}
    QList<QString> &characterNames() { return m_characterNames; }
    ConversationContext *rootContext();
    QPointer<ConversationContext> getContextByID(int id);

    QPointer<ConversationContext> selectContext(QWidget *parent);

    QSet<int> &usedIDs() { return m_usedIDs; }

    int getAvailableID();

    void edit(QWidget *parent);

    void serialize(QXmlStreamWriter &xml);
    void deserialize(QDomDocument &doc);
private:
    QStandardItem *makeContextItem(ConversationContext *context,
        bool editable);
    QStandardItemModel *makeContextModel(QMap<int, QStandardItem *> &items,
        bool editable);
};

#endif
