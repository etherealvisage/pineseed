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
                m_current->setData(speaker, SpeakerData);
                updateActionTitle(m_current);
            }});
    speechLayout->addWidget(m_currentSpeaker);

    m_currentSpeech = new QTextEdit();
    m_currentSpeech->setAcceptRichText(false);
    connect(m_currentSpeech, &QTextEdit::textChanged, 
        [=]() {
            if(m_current) {
                m_current->setData(m_currentSpeech->toPlainText(), SpeechData);
                updateActionTitle(m_current);
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

    item->setData(TypeData, Empty);

    updateActionTitle(item);
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
    m_current->setData(to, TypeData);
    updateActionTitle(m_current);
    if(to == Sequence || to == Concurrent || to == Conditional) 
        m_current->setDropEnabled(true);
    else
        m_current->setDropEnabled(false);
}

void ActionEditor::updateActionTitle(QStandardItem *item) {
    QString title;
    switch((ActionType)item->data(TypeData).toInt()) {
    case Empty:
        title = "[empty]";
        break;
    case Speech: {
        QString speaker = item->data(SpeakerData).toString();
        QString speech = item->data(SpeechData).toString();
        int ind = speech.indexOf('\n');
        if(ind != -1) speech.truncate(ind);
        if(speech.length() > 30) {
            speech.truncate(30);
            speech += "...";
        }
        title = "[speech] " + speaker + ": " + speech;
        break;
    }
    case Emote:
        title = "[emote]";
        break;
    case Sequence:
        title = "[sequence]";
        break;
    case Concurrent:
        title = "[concurrent]";
        break;
    case Conditional:
        title = "[conditional]";
        break;
    case Jump:
        title = "[jump]";
        break;
    case EndConversation:
        title = "[end]";
        break;
    }
    item->setData(title, Qt::DisplayRole);
}

void ActionEditor::changeTo(QStandardItem *item) {
    m_current = item;
    if(!item) {
        m_currentType->setEnabled(false);
        m_currentStack->setEnabled(false);
    }
    else {
        m_currentType->setEnabled(true);
        m_currentType->setCurrentIndex(item->data(TypeData).toInt());

        m_currentStack->setEnabled(true);
        m_currentStack->setCurrentIndex(item->data(TypeData).toInt());

        m_currentSpeaker->setText(item->data(SpeakerData).toString());

        m_currentSpeech->setText(item->data(SpeechData).toString());
    }
}
