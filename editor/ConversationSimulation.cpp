#include <QComboBox>
#include <QListWidget>
#include <QVBoxLayout>
#include <QStandardItem>
#include <QVector>

#include "ConversationSimulation.h"
#include "moc_ConversationSimulation.cpp"

#include "Link.h"
#include "ActionEditor.h"
#include "Node.h"

ConversationSimulation::ConversationSimulation() {
    QVBoxLayout *layout = new QVBoxLayout();
    m_history = new QListWidget();
    m_history->setWrapping(true);
    m_history->setWordWrap(true);
    layout->addWidget(m_history);
    m_options = new QComboBox();
    m_options->setEditable(false);
    layout->addWidget(m_options);
    setLayout(layout);

    connect(m_options, SIGNAL(activated(const QString &)),
        this, SLOT(progress(const QString &)));
}

void ConversationSimulation::beginFrom(Node *node) {
    m_history->clear();
    // TODO: set up initial state etc.
    process(node);
}

void ConversationSimulation::progress(const QString &by) {
    process(m_optionsMap[by]);
}

void ConversationSimulation::process(Node *node) {
    m_current = node;

    // perform actions
    QStandardItem *root = node->actionModel()->invisibleRootItem();
    for(int i = 0; i < root->rowCount(); i ++) {
        if(process(root->child(i))) return;
    }

    // collate options
    m_options->clear();
    auto links = node->links();
    for(auto link : links) {
        if(link->from() != node) continue;

        m_options->addItem(link->label());
        m_optionsMap[link->label()] = link->to();
    }

    m_history->scrollToBottom();
}

bool ConversationSimulation::process(QStandardItem *action) {
    ActionEditor::ActionType type =
        (ActionEditor::ActionType)action->data(ActionEditor::TypeData).toInt();

    switch(type) {
    case ActionEditor::Empty:
        break;
    case ActionEditor::Speech: {
        QListWidgetItem *item = new QListWidgetItem();
        const QString &speaker =
            action->data(ActionEditor::SpeakerData).toString();
        item->setText(speaker + " says: "
            + action->data(ActionEditor::SpeechData).toString());
        quint16 hash = qHash(speaker);
        item->setBackgroundColor(qRgb(240 + ((hash>>8)&0xf),
            240 + ((hash>>4)&0xf), 240 + ((hash)&0xf)));
        m_history->addItem(item);
        break;
    }
    case ActionEditor::Emote:
        break;
    case ActionEditor::Sequence:
        break;
    case ActionEditor::Concurrent:
        break;
    case ActionEditor::Conditional:
        break;
    case ActionEditor::Jump:
        break;
    case ActionEditor::EndConversation:
        m_current = nullptr;
        return true;
    }

    for(int i = 0; i < action->rowCount(); i ++) {
        if(process(action->child(i))) return true;
    }
    return false;
}
