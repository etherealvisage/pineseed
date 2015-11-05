#ifndef ConversationContext_H
#define ConversationContext_H

#include <QObject>
#include <QString>

class Node;

// is a QObject so we can use QPointer
class ConversationContext : public QObject { Q_OBJECT
private:
    int m_id;
    QString m_label;
public:
    ConversationContext(int id) : m_id(id) {}

    int id() const { return m_id; }

    QString label() const { return m_label; }
    void setLabel(QString label) { m_label = label; }
};

#endif
