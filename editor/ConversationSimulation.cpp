#include <QComboBox>
#include <QListWidget>
#include <QVBoxLayout>
#include <QStandardItem>
#include <QVector>

#include "ConversationSimulation.h"
#include "moc_ConversationSimulation.cpp"

#include "Link.h"
#include "Action.h"
#include "Node.h"

ConversationSimulation::ConversationSimulation() {
    QVBoxLayout *layout = new QVBoxLayout();
    m_history = new QListWidget();
    //m_history->setWrapping(true);
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
    if(!node) {
        m_options->clear();
        m_optionsMap.clear();

        QListWidgetItem *item = new QListWidgetItem();
        item->setText("[Finished]");
        item->setTextAlignment(Qt::AlignHCenter);
        item->setBackgroundColor(qRgb(255, 224, 224));
        m_history->addItem(item);

        return;
    }

    // perform actions
    QStandardItem *root = node->actionModel()->invisibleRootItem();
    for(int i = 0; i < root->rowCount(); i ++) {
        if(process(root->child(i))) return;
    }

    // collate options
    m_options->clear();
    m_optionsMap.clear();
    auto links = node->links();
    for(auto link : links) {
        if(link->from() != node) continue;

        m_options->addItem(link->label());
        m_optionsMap[link->label()] = link->to();
    }

    m_history->scrollToBottom();
}

bool ConversationSimulation::process(QStandardItem *action) {
    Action::ActionType type =
        (Action::ActionType)action->data(Action::TypeData).toInt();

    switch(type) {
    case Action::Empty:
        break;
    case Action::Speech: {
        QListWidgetItem *item = new QListWidgetItem();
        const QString &speaker =
            action->data(Action::SpeakerData).toString();
        item->setText(speaker + " says: "
            + action->data(Action::SpeechData).toString());
        quint16 hash = qHash(speaker);
        item->setBackgroundColor(qRgb(224 + ((hash>>8)&0xf),
            224 + ((hash>>4)&0xf), 224 + ((hash)&0xf)));
        m_history->addItem(item);
        break;
    }
    case Action::Emote:
        break;
    case Action::Sequence:
        break;
    case Action::Concurrent:
        break;
    case Action::Conditional:
        break;
    case Action::Jump: {
        Node *target =
            (Node *)action->data(Action::JumpTargetData).value<void *>();
        if(target) {
            process(target);
            return true;
        }
        break;
    }
    case Action::EndConversation:
        process((Node *)nullptr);
        return true;
    }

    for(int i = 0; i < action->rowCount(); i ++) {
        if(process(action->child(i))) return true;
    }
    return false;
}
