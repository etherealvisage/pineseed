#include <QComboBox>
#include <QListWidget>
#include <QVBoxLayout>
#include <QStandardItem>
#include <QList>
#include <QLineEdit>
#include <QPointer>

#include "ConversationSimulation.h"
#include "moc_ConversationSimulation.cpp"

#include "Link.h"
#include "Action.h"
#include "Node.h"
#include "ConversationContext.h"

ConversationSimulation::ConversationSimulation() {
    QVBoxLayout *layout = new QVBoxLayout();
    m_history = new QListWidget();
    m_history->setWordWrap(true);
    layout->addWidget(m_history);
    m_options = new QComboBox();
    m_options->setEditable(false);
    layout->addWidget(m_options);
    m_entryBox = new QLineEdit();
    connect(m_entryBox, &QLineEdit::returnPressed,
        [=]() {
            auto text = m_entryBox->text();
            if(m_optionsMap.contains(text)) {
                progress(text);
                m_entryBox->clear();
            }
        });
    layout->addWidget(m_entryBox);
    setLayout(layout);

    connect(m_options, SIGNAL(activated(int)),
        this, SLOT(progress(int)));
}

void ConversationSimulation::beginFrom(Node *node) {
    m_history->clear();
    // TODO: set up initial state etc.
    m_visited.clear();
    m_returns.clear();
    process(node);
}

void ConversationSimulation::progress(int index) {
    progress(m_options->itemData(index, Qt::UserRole).toString());
}

void ConversationSimulation::progress(const QString &by) {
    if(by == "") return;
    auto link = m_optionsMap[by.toLower()];
    if(link->isRtsLink()) m_returns.push_back(m_current);
    process(link->to());
}

void ConversationSimulation::process(Node *node, bool supress) {
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
    if(!supress) {
        QStandardItem *root = node->actionModel()->invisibleRootItem();
        for(int i = 0; i < root->rowCount(); i ++) {
            if(process(root->child(i))) return;
        }
    }

    // collate options
    m_options->clear();
    m_options->addItem("");
    m_optionsMap.clear();
    // start from node links
    auto links = node->links();
    for(auto link : links) {
        if(link->from() != node) continue;

        if(link->isHiddenLink()) {
            m_options->addItem(link->label() + " (hidden)",
                link->label().toLower());
        }
        else m_options->addItem(link->label(), link->label().toLower());
        m_optionsMap[link->label().toLower()] = link;
    }
    // try adding context links, if appropriate
    QPointer<ConversationContext> context = node->context();
    while(!context.isNull()) {
        for(auto link : context->links()) {
            if(m_optionsMap.contains(link->label())) continue;

            if(link->isHiddenLink())
                m_options->addItem(link->label() + " (hidden, context)",
                    link->label().toLower());
            else
                m_options->addItem(link->label() + " (context)",
                    link->label().toLower());
            m_optionsMap[link->label().toLower()] = link;
        }
        context = context->parent();
    }

    m_history->scrollToBottom();
    m_visited.insert(node);
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
            action->data(Action::ActorData).toString();
        item->setText(speaker + " says: "
            + action->data(Action::SpeechData).toString());
        quint16 hash = qHash(speaker);
        item->setBackgroundColor(qRgb(224 + ((hash>>8)&0xf),
            224 + ((hash>>4)&0xf), 224 + ((hash)&0xf)));
        m_history->addItem(item);
        break;
    }
    case Action::Emote: {
        QListWidgetItem *item = new QListWidgetItem();
        const QString &actor=
            action->data(Action::ActorData).toString();
        item->setText("* " + actor + " "
            + action->data(Action::EmoteData).toString());
        quint16 hash = qHash(actor);
        item->setBackgroundColor(qRgb(224 + ((hash>>8)&0xf),
            224 + ((hash>>4)&0xf), 224 + ((hash)&0xf)));
        m_history->addItem(item);
        break;
    }
    case Action::Sequence: {
        for(int i = 0; i < action->rowCount(); i ++) {
            process(action->child(i));
        }
        break;
    }
    case Action::Concurrent: {
        for(int i = 0; i < action->rowCount(); i ++) {
            process(action->child(i));
        }
        break;
    }
    case Action::Conditional:
        break;
    case Action::Jump: {
        QPointer<Node> *target = (QPointer<Node> *)action->data(
            Action::JumpTargetData).value<void *>();

        if(target && target->data()) {
            process(target->data());
            return true;
        }
        break;
    }
    case Action::EndConversation:
        process((Node *)nullptr);
        return true;
    case Action::FirstVisitConditional: {
        bool invert = action->data(Action::ConditionalInversionData).toBool();
        if(m_visited.contains(m_current) == invert) {
            for(int i = 0; i < action->rowCount(); i ++) {
                process(action->child(i));
            }
        }
        break;
    }
    case Action::ReturnToSender: {
        if(m_returns.size() == 0) break;
        process(m_returns.takeLast(), true);
        return true;
    }
    case Action::RandomChoice: {
        if(action->rowCount() == 0) return false;
        process(action->child(qrand() % action->rowCount()));
        return true;
    }
    case Action::ActionTypes:
    default:
        qDebug("Unknown action type encountered in simulation!");
        return false;
    }

    return false;
}
