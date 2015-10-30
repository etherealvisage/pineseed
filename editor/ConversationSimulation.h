#ifndef ConversationSimulation_H
#define ConversationSimulation_H

#include <QWidget>

class QComboBox;
class QListWidget;
class QStandardItem;

class Node;

class ConversationSimulation : public QWidget { Q_OBJECT
private:
    Node *m_current;
    QListWidget *m_history;
    QComboBox *m_options;
public:
    ConversationSimulation();
public slots:
    void beginFrom(Node *node);
private slots:
    void progress(const QString &by);
    void process(Node *node);
    // returns true if m_current is changed by the action
    bool process(QStandardItem *action);
};

#endif
