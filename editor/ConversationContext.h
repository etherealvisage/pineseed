#ifndef ConversationContext_H
#define ConversationContext_H

#include <QObject>
#include <QString>
#include <QPointer>
#include <QSet>

class Node;
class Link;
class QBrush;

// is a QObject so we can use QPointer
class ConversationContext : public QObject { Q_OBJECT
private:
    int m_id;
    QString m_label;
    QPointer<ConversationContext> m_parent;
    QSet<Link *> m_links;
public:
    ConversationContext(int id) : m_id(id) {}

    int id() const { return m_id; }

    QString label() const { return m_label; }
    void setLabel(QString label) { m_label = label; }

    QPointer<ConversationContext> parent() const { return m_parent; }
    void setParent(QPointer<ConversationContext> parent) { m_parent = parent; }

    QSet<Link *> &links() { return m_links; }
    const QSet<Link *> &links() const { return m_links; }


    QBrush deriveBrush() const;
};

#endif
