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
#include <QFileDialog>
#include <QFile>
#include <QXmlStreamWriter>
#include <QStandardItemModel>
#include <QDomDocument>

#include "ConversationWindow.h"
#include "moc_ConversationWindow.cpp"

#include "Node.h"
#include "Link.h"
#include "ActionEditor.h" // for ItemData definitions
#include "ConversationSimulation.h"

ConversationWindow::ConversationWindow() {
    m_split = new QSplitter();
    m_edit = new QWidget();
    m_split->addWidget(m_edit);
    m_cview = new ConversationView();
    m_split->addWidget(m_cview);
    m_sim = new ConversationSimulation();
    m_split->addWidget(m_sim);
    modeChange(SelectMode); // enter select mode by default

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

    QPushButton *simulateButton = new QPushButton(tr("Simulate"));
    connect(simulateButton, SIGNAL(clicked(bool)), m_modeMapper, SLOT(map()));
    m_modeMapper->setMapping(simulateButton, SimulateMode);
    editbarLayout->addWidget(simulateButton);
    m_toolButtons.push_back(simulateButton);
    QShortcut *simulateShortcut = new QShortcut(QKeySequence("Alt+5"), this);
    connect(simulateShortcut, SIGNAL(activated()), simulateButton,
        SLOT(click()));

    editLayout->addLayout(editbarLayout);

    m_editarea = new QWidget();
    m_editarea->setLayout(new QFormLayout());
    editLayout->addWidget(m_editarea);

    m_edit->setLayout(editLayout);

    m_selectLast = nullptr;
}

void ConversationWindow::save() {
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save conversation"));
    QFile file(filename);
    if(!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) return;
    QFileInfo fi(filename);
    setWindowTitle(fi.baseName());

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();

    auto items = m_cview->scene()->items();
    QMap<ConversationObject *, int> itemID;

    for(auto item : items) {
        auto obj = dynamic_cast<ConversationObject *>(item);
        if(!obj) continue;
        itemID[obj] = itemID.size();
    }

    xml.writeStartElement("objects");
    for(auto item : items) {
        auto obj = dynamic_cast<ConversationObject *>(item);
        if(!obj) continue;

        obj->serialize(xml, itemID);
    }
    xml.writeEndElement(); // objects

    xml.writeEndDocument();
}

void ConversationWindow::load() {
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open conversation"));
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) return;
    QFileInfo fi(filename);
    setWindowTitle(fi.baseName());
    
    QDomDocument doc;
    doc.setContent(&file);

    auto objectss = doc.elementsByTagName("objects");
    if(objectss.size() != 1) {
        qDebug("Expecting exactly one <objects>");
        return;
    }
    auto objects = objectss.at(0);

    QMap<int, ConversationObject *> objs;
    auto nodes = objects.childNodes();
    for(int i = 0; i < nodes.length(); i ++) {
        auto element = nodes.at(i).toElement();

        auto name = element.tagName();
        auto id = element.attribute("id").toInt();

        if(name == "node") {
            objs[id] = new Node();
        }
        else if(name == "link") {
            objs[id] = new Link(nullptr, nullptr);
        }
        else {
            qDebug("Unknown object type %s", name.toLocal8Bit().constData());
        }
    }

    for(int i = 0; i < nodes.length(); i ++) {
        auto element = nodes.at(i).toElement();

        auto id = element.attribute("id").toInt();
        
        objs[id]->deserialize(element, objs);
        m_cview->scene()->addItem(objs[id]);
    }
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
    case SimulateMode:
        m_cview->enterSelectMode();
        connect(m_cview, SIGNAL(selected(ConversationObject *)),
            this, SLOT(beginSimulation(ConversationObject *)));
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
    if(m_selectLast) m_selectLast->deselect();
    m_selectLast = object;
    if(m_selectLast) m_selectLast->select();
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

void ConversationWindow::beginSimulation(ConversationObject *object) {
    Node *node = dynamic_cast<Node *>(object);
    if(!node) return;

    m_sim->beginFrom(node);
    modeChange(SelectMode);
    selectObject(object);
}
