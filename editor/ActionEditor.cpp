#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>

#include "ActionEditor.h"
#include "moc_ActionEditor.cpp"

#include "Action.h"

ActionEditor::ActionEditor(QStandardItemModel *model) : m_model(model) {
    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);

    m_actionView = new QTreeView();
    m_actionView->setModel(m_model);
    connect(m_actionView->selectionModel(),
        SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
        this, SLOT(currentChanged(const QModelIndex &, const QModelIndex &)));
    layout->addWidget(m_actionView);
    m_actionView->setDragDropMode(QAbstractItemView::InternalMove);

    QPushButton *addButton = new QPushButton(tr("Add action"));
    layout->addWidget(addButton);
    connect(addButton, SIGNAL(clicked(bool)), this, SLOT(addAction()));
    QPushButton *removeButton = new QPushButton(tr("Remove action"));
    layout->addWidget(removeButton);
    connect(removeButton, SIGNAL(clicked(bool)), this, SLOT(removeAction()));

    m_currentType = new QComboBox();
    m_currentType->addItem(tr("Empty"));
    m_currentType->addItem(tr("Speech"));
    m_currentType->addItem(tr("Emote"));
    m_currentType->addItem(tr("Sequence"));
    m_currentType->addItem(tr("Concurrent"));
    m_currentType->addItem(tr("Conditional"));
    m_currentType->addItem(tr("Jump"));
    m_currentType->addItem(tr("End conversation"));
    layout->addWidget(m_currentType);

    m_currentStack = new QStackedWidget();
    layout->addWidget(m_currentStack);
    connect(m_currentType, SIGNAL(activated(int)),
        this, SLOT(changeType(int)));

    m_currentStack->addWidget(new QLabel("Please select a type"));

    QVBoxLayout *speechLayout = new QVBoxLayout();
    m_currentSpeaker = new QLineEdit();
    connect(m_currentSpeaker, &QLineEdit::textChanged, 
        [=](const QString &speaker) {
            if(m_current) {
                m_current->setData(speaker, Action::SpeakerData);
                Action::updateTitle(m_current);
            }});
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

    m_currentStack->addWidget(new QLabel("Emote"));
    m_currentStack->addWidget(new QLabel("Sequence"));
    m_currentStack->addWidget(new QLabel("Concurrent"));
    m_currentStack->addWidget(new QLabel("Conditional"));
    m_currentStack->addWidget(new QLabel("Jump"));
    m_currentStack->addWidget(new QLabel("End conversation"));
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
    if(to == Action::Sequence || to == Action::Concurrent
        || to == Action::Conditional) m_current->setDropEnabled(true);
    else m_current->setDropEnabled(false);
}

void ActionEditor::changeTo(QStandardItem *item) {
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

        m_currentSpeaker->setText(item->data(Action::SpeakerData).toString());
        m_currentSpeech->setText(item->data(Action::SpeechData).toString());
    }
}
