#ifndef ConversationSimulation_H
#define ConversationSimulation_H

#include <QMap>
#include <QWidget>
#include <QSet>

class QComboBox;
class QListWidget;
class QStandardItem;

class Node;
class Link;

class ConversationSimulation : public QWidget { Q_OBJECT
private:
    Node *m_current;
    QListWidget *m_history;
    QComboBox *m_options;
    QMap<QString, Link *> m_optionsMap;
    QSet<Node *> m_visited;
    QMap<Node *, Node *> m_returns;
public:
    ConversationSimulation();
public slots:
    void beginFrom(Node *node);
private slots:
    void progress(const QString &by);
    void process(Node *node, bool supress = false);
    // returns true if m_current is changed by the action
    bool process(QStandardItem *action);
};

#endif
