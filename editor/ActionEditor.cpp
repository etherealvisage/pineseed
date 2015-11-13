#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QCheckBox>
#include <QPointer>

#include "ActionEditor.h"
#include "moc_ActionEditor.cpp"

#include "Action.h"
#include "ConversationData.h"
#include "ConversationDataInterface.h"
#include "Node.h"

ActionEditor::ActionEditor(ConversationDataInterface *interface,
    ConversationData *data, QStandardItemModel *model)
    : m_dataInterface(interface), m_data(data), m_model(model) {

    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);

    // TODO: go over each item in model and call updateTitle().

    m_actionView = new QTreeView();
    m_actionView->setHeaderHidden(true);
    m_actionView->setModel(m_model);
    connect(m_actionView->selectionModel(),
        SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
        this, SLOT(currentChanged(const QModelIndex &, const QModelIndex &)));
    layout->addWidget(m_actionView);
    m_actionView->setDragDropMode(QAbstractItemView::InternalMove);

    auto addButton = new QPushButton(tr("Add action"));
    connect(addButton, SIGNAL(clicked(bool)), this, SLOT(addAction()));
    auto removeButton = new QPushButton(tr("Remove action"));
    connect(removeButton, SIGNAL(clicked(bool)), this, SLOT(removeAction()));

    auto arLayout = new QHBoxLayout();
    arLayout->addWidget(addButton);
    arLayout->addWidget(removeButton);
    layout->addLayout(arLayout);

    m_currentType = new QComboBox();
    layout->addWidget(m_currentType);

    m_currentStack = new QStackedWidget();
    layout->addWidget(m_currentStack);
    connect(m_currentType, SIGNAL(activated(int)),
        this, SLOT(changeType(int)));

    layout->addWidget(new QLabel("Action comments:"));
    m_currentComment = new QTextEdit();
    m_currentComment->setAcceptRichText(false);
    layout->addWidget(m_currentComment);

    { // Empty
        m_currentType->addItem(tr("Empty"));
        m_currentStack->addWidget(new QLabel("Please select a type"));
    }

    { // Speech
        m_currentType->addItem(tr("Speech"));
        QVBoxLayout *speechLayout = new QVBoxLayout();
        m_currentSpeaker = new QComboBox();
        connect(m_currentSpeaker,
            static_cast<void (QComboBox::*)(const QString &)>(
                &QComboBox::activated),
            [=](const QString &speaker) {
                if(m_current) {
                    m_current->setData(speaker, Action::ActorData);
                    Action::updateTitle(m_current);
                }});
        connect(m_currentSpeaker,
            &QComboBox::currentTextChanged,
            [=](const QString &actor) {
                m_currentEmoter->setCurrentText(actor);
            });
        speechLayout->addWidget(m_currentSpeaker);

        m_currentSpeech = new QTextEdit();
        m_currentSpeech->setAcceptRichText(false);
        connect(m_currentSpeech, &QTextEdit::textChanged, 
            [=]() {
                if(m_current) {
                    m_current->setData(m_currentSpeech->toPlainText(),
                        Action::SpeechData);
                    Action::updateTitle(m_current);
                }
            });
        speechLayout->addWidget(m_currentSpeech);
        QWidget *speechWidget = new QWidget();
        speechWidget->setLayout(speechLayout);
        m_currentStack->addWidget(speechWidget);
    }

    { // Emote
        m_currentType->addItem(tr("Emote"));
        QVBoxLayout *emoteLayout = new QVBoxLayout();
        m_currentEmoter = new QComboBox();
        m_currentEmoter->setModel(m_currentSpeaker->model());
        connect(m_currentEmoter,
            static_cast<void (QComboBox::*)(const QString &)>(
                &QComboBox::activated),
            [=](const QString &actor) {
                if(m_current) {
                    m_current->setData(actor, Action::ActorData);
                    Action::updateTitle(m_current);
                }});
        connect(m_currentEmoter,
            &QComboBox::currentTextChanged,
            [=](const QString &actor) {
                m_currentSpeaker->setCurrentText(actor);
            });
        emoteLayout->addWidget(m_currentEmoter);

        m_currentEmote = new QTextEdit();
        m_currentEmote->setAcceptRichText(false);
        connect(m_currentEmote, &QTextEdit::textChanged, 
            [=]() {
                if(m_current) {
                    m_current->setData(m_currentEmote->toPlainText(),
                        Action::EmoteData);
                    Action::updateTitle(m_current);
                }
            });
        emoteLayout->addWidget(m_currentEmote);
        QWidget *emoteWidget = new QWidget();
        emoteWidget->setLayout(emoteLayout);
        m_currentStack->addWidget(emoteWidget);
    }
    { // Sequence
        m_currentType->addItem(tr("Action sequence"));
        m_currentStack->addWidget(new QLabel("Sequence"));
    }
    { // Concurrent
        m_currentType->addItem(tr("Concurrent actions"));
        m_currentStack->addWidget(new QLabel("Concurrent"));
    }
    { // Conditional
        m_currentType->addItem(tr("Conditional actions"));
        m_currentStack->addWidget(new QLabel("Conditional"));
    }
    { // Jump
        m_currentType->addItem(tr("Jump"));
        QVBoxLayout *jumpLayout = new QVBoxLayout();
        m_currentJumpTarget = new QLabel();
        jumpLayout->addWidget(m_currentJumpTarget);
        QPushButton *changeJumpTarget = new QPushButton(tr("Change target"));

        connect(changeJumpTarget, &QPushButton::clicked,
            [=](){
                m_dataInterface->selectObjects(
                    std::function<bool (ConversationObject *)>(
                    [=](ConversationObject *object) {
                        return dynamic_cast<Node *>(object);
                    }),
                    std::function<bool (ConversationObject *)>(
                    [=](ConversationObject *object) {
                        m_current->setData(qVariantFromValue(
                                (void *)new QPointer<Node>((Node *)object)),
                            Action::JumpTargetData);
                        Action::updateTitle(m_current);
                        return true;
                    }));
            });
        jumpLayout->addWidget(changeJumpTarget);
        QWidget *jumpWidget = new QWidget();
        jumpWidget->setLayout(jumpLayout);
        m_currentStack->addWidget(jumpWidget);
    }
    { // End
        m_currentType->addItem(tr("End conversation"));
        m_currentStack->addWidget(new QLabel("End conversation"));
    }
    { // First visit conditional
        m_currentType->addItem(tr("First-visit conditional"));
        m_currentConditionalInversion = new QCheckBox(tr("Invert?"));
        connect(m_currentConditionalInversion, &QCheckBox::stateChanged,
            [=](int newState){
                if(m_current) {
                    m_current->setData(newState == Qt::Checked,
                        Action::ConditionalInversionData);
                    Action::updateTitle(m_current);
                }
            });
        m_currentStack->addWidget(m_currentConditionalInversion);
    }
    { // Return to sender
        m_currentType->addItem(tr("Return to sender"));
        m_currentStack->addWidget(new QLabel("Return to sender"));
    }
    { // Random choice
        m_currentType->addItem(tr("Random choice"));
        m_currentStack->addWidget(new QLabel("Random choice"));
    }

    m_currentStack->addWidget(new QLabel("error!"));

    changeTo(nullptr);
}

void ActionEditor::addAction() {
    QStandardItem *parent = m_model->invisibleRootItem();

    QStandardItem *item = new QStandardItem(QString(""));
    parent->appendRow(item);

    item->setEditable(false);
    item->setDropEnabled(false);

    item->setData(Action::TypeData, Action::Empty);

    Action::updateTitle(item);
    changeTo(item);
}

void ActionEditor::removeAction() {
    auto tr = m_current;
    changeTo(nullptr);
    if(tr) {
        if(tr->parent()) tr->parent()->removeRow(tr->row());
        else m_model->removeRow(tr->row());
    }
}

void ActionEditor::currentChanged(const QModelIndex &now,
    const QModelIndex &before) {

    auto item = m_model->itemFromIndex(now);
    changeTo(item);
}

void ActionEditor::changeType(int to) {
    m_currentStack->setCurrentIndex(to);
    m_current->setData(to, Action::TypeData);
    Action::updateTitle(m_current);
    if(Action::isContainer((Action::ActionType)to)) {
        m_current->setDropEnabled(true);
    }
    else m_current->setDropEnabled(false);
}

void ActionEditor::changeTo(QStandardItem *item) {
    // refresh current speaker options
    m_currentSpeaker->clear();
    for(auto sp : m_data->characterNames()) {
        m_currentSpeaker->addItem(sp);
    }

    m_current = item;
    if(!item) {
        m_currentType->setEnabled(false);
        m_currentStack->setEnabled(false);
    }
    else {
        int type = item->data(Action::TypeData).toInt();
        m_currentType->setEnabled(true);
        m_currentType->setCurrentIndex(type);

        m_currentStack->setEnabled(true);
        m_currentStack->setCurrentIndex(type);

        m_currentSpeaker->setCurrentText(
            item->data(Action::ActorData).toString());
        m_currentSpeech->setText(item->data(Action::SpeechData).toString());

        m_currentEmoter->setCurrentText(
            item->data(Action::ActorData).toString());
        m_currentEmote->setText(item->data(Action::EmoteData).toString());

        //m_currentJumpTarget->setText(item->data(Action::JumpTargetData));
    }
}
