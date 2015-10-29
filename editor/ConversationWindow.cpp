#include <QAction>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QSplitter>
#include <QPushButton>
#include <QFormLayout>
#include <QShortcut>
#include <QKeySequence>

#include "ConversationWindow.h"
#include "moc_ConversationWindow.cpp"

#include "Node.h"
#include "Link.h"

ConversationWindow::ConversationWindow() {
    m_split = new QSplitter();
    m_edit = new QWidget();
    m_split->addWidget(m_edit);
    m_cview = new ConversationView();
    m_split->addWidget(m_cview);
    
    setWidget(m_split);

    QVBoxLayout *editLayout = new QVBoxLayout();

    m_modeMapper = new QSignalMapper();
    connect(m_modeMapper, SIGNAL(mapped(int)), this, SLOT(modeChange(int)));
    QHBoxLayout *editbarLayout = new QHBoxLayout();

    QShortcut *selectModeShortcut = new QShortcut(QKeySequence("Alt+`"), this);
    connect(selectModeShortcut, &QShortcut::activated,
        [=](){ modeChange(SelectMode); });

    QPushButton *newNodeButton = new QPushButton(tr("Node"));
    connect(newNodeButton, SIGNAL(clicked(bool)), m_modeMapper, SLOT(map()));
    m_modeMapper->setMapping(newNodeButton, NewNodeMode);
    editbarLayout->addWidget(newNodeButton);
    m_toolButtons.push_back(newNodeButton);
    QShortcut *newNodeShortcut = new QShortcut(QKeySequence("Alt+1"), this);
    connect(newNodeShortcut, SIGNAL(activated()),
        newNodeButton, SLOT(click()));

    QPushButton *newLinkButton = new QPushButton(tr("Link"));
    connect(newLinkButton, SIGNAL(clicked(bool)), m_modeMapper, SLOT(map()));
    m_modeMapper->setMapping(newLinkButton, NewLinkMode);
    editbarLayout->addWidget(newLinkButton);
    m_toolButtons.push_back(newLinkButton);
    QShortcut *newLinkShortcut = new QShortcut(QKeySequence("Alt+2"), this);
    connect(newLinkShortcut, SIGNAL(activated()),
        newLinkButton, SLOT(click()));

    QPushButton *newContextButton = new QPushButton(tr("Context"));
    connect(newContextButton, SIGNAL(clicked(bool)), m_modeMapper,
        SLOT(map()));
    m_modeMapper->setMapping(newContextButton, NewContextMode);
    editbarLayout->addWidget(newContextButton);
    m_toolButtons.push_back(newContextButton);
    QShortcut *newContextShortcut = new QShortcut(QKeySequence("Alt+3"), this);
    connect(newContextShortcut, SIGNAL(activated()),
        newContextButton, SLOT(click()));

    QPushButton *deleteButton = new QPushButton(tr("Delete"));
    connect(deleteButton, SIGNAL(clicked(bool)), m_modeMapper, SLOT(map()));
    m_modeMapper->setMapping(deleteButton, DeleteMode);
    editbarLayout->addWidget(deleteButton);
    m_toolButtons.push_back(deleteButton);
    QShortcut *deleteShortcut = new QShortcut(QKeySequence("Alt+4"), this);
    connect(deleteShortcut, SIGNAL(activated()), deleteButton, SLOT(click()));

    editLayout->addLayout(editbarLayout);

    m_editarea = new QWidget();
    m_editarea->setLayout(new QFormLayout());
    editLayout->addWidget(m_editarea);

    m_edit->setLayout(editLayout);

    m_selectLast = nullptr;
}

void ConversationWindow::modeChange(int to) {
    Mode mode = (Mode)to;

    m_cview->disconnect(m_cview, SIGNAL(clicked(QPointF)), this, 0);
    m_cview->disconnect(m_cview, SIGNAL(selected(ConversationObject *)),
        this, 0);

    for(auto b : m_toolButtons) if(!b->isEnabled()) b->setEnabled(true);
    if(mode != SelectMode) m_toolButtons[to-1]->setEnabled(false);
    switch(mode) {
    case SelectMode:
        m_cview->enterSelectMode();
        connect(m_cview, SIGNAL(selected(ConversationObject *)),
            this, SLOT(selectObject(ConversationObject *)));
        break;
    case NewNodeMode:
        m_cview->enterInsertMode();
        connect(m_cview, SIGNAL(clicked(QPointF)),
            this, SLOT(insertNode(QPointF)));
        break;
    case NewContextMode:
        m_cview->enterInsertMode();
        break;
    case NewLinkMode:
        if(!m_selectLast || !dynamic_cast<Node *>(m_selectLast)) {
            modeChange(SelectMode);
        }
        else {
            m_cview->enterSelectMode();
            connect(m_cview, SIGNAL(selected(ConversationObject *)),
                this, SLOT(makeLink(ConversationObject *)));
        }
        break;
    case DeleteMode:
        m_cview->enterSelectMode();
        connect(m_cview, SIGNAL(selected(ConversationObject *)),
            this, SLOT(deleteObject(ConversationObject *)));
        break;
    default:
        qFatal("Unexpected mode change value");
        break;
    }
}

void ConversationWindow::selectObject(ConversationObject *object) {
    while(m_editarea->layout()->count() > 0) {
        auto item = m_editarea->layout()->takeAt(0);
        if(item->widget()) item->widget()->deleteLater();
        if(item->layout()) delete item->layout();

        delete item;
    }
    if(object) object->edit(dynamic_cast<QFormLayout *>(m_editarea->layout()));
    m_selectLast = object;
}

void ConversationWindow::insertNode(QPointF where) {
    Node *node = new Node();
    node->setPos(where - node->boundingRect().center());
    m_cview->scene()->addItem(node);

    connect(node, SIGNAL(changed()), m_cview->viewport(), SLOT(update()));
    selectObject(node);
    modeChange(SelectMode);
}

void ConversationWindow::insertContext(QPointF where) {
    qDebug("Contexts NYI!");
}

void ConversationWindow::makeLink(ConversationObject *object) {
    Node *target = dynamic_cast<Node *>(object);
    // not allowed to have self-links for now
    if(!target || target == m_selectLast) return; 

    Link *link = new Link(dynamic_cast<Node *>(m_selectLast), target);
    connect(link, SIGNAL(changed()), m_cview->viewport(), SLOT(update()));
    m_cview->scene()->addItem(link);

    selectObject(link);
    modeChange(SelectMode);
}

void ConversationWindow::deleteObject(ConversationObject *object) {
    object->deleteLater();

    modeChange(SelectMode);
    selectObject(nullptr);
}
