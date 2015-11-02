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
#include "ConversationDataInterface.h"

ConversationWindow::ConversationWindow() {
    m_data = new ConversationData();

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

    editLayout->addLayout(editbarLayout);

    m_editarea = new QWidget();
    m_editarea->setLayout(new QFormLayout());
    editLayout->addWidget(m_editarea);

    m_simbutton = new QPushButton(tr("Simulate"));
    m_simbutton->setEnabled(false);
    connect(m_simbutton, SIGNAL(clicked(bool)),
        this, SLOT(beginSimulation()));
    editLayout->addStretch(1);
    editLayout->addWidget(m_simbutton);

    QPushButton *dataButton = new QPushButton(tr("Conversation data"));
    connect(dataButton, &QPushButton::clicked, [=](){ m_data->edit(this); });
    editLayout->addWidget(dataButton);

    m_edit->setLayout(editLayout);

    m_selectLast = nullptr;

    class InternalInterface : public ConversationDataInterface {
    private:
        ConversationWindow *m_window;
    public:
        InternalInterface(ConversationWindow *window) : m_window(window) {}

        virtual void selectObject(
            std::function<bool (ConversationObject *)> filter,
            std::function<void (ConversationObject *)> callback) {

            m_window->m_selectOneFilter = filter;
            m_window->m_selectOneCallback = callback;

            m_window->modeChange(SelectOneMode);
        }
    };

    m_dataInterface = new InternalInterface(this);
}

void ConversationWindow::saveTo(QFile &file) {
    /*QString filename = QFileDialog::getSaveFileName(this,
        tr("Save conversation"));
    QFile file(filename);
    if(!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) return;
    QFileInfo fi(filename);
    setWindowTitle(fi.baseName());*/
    QFileInfo fi(file.fileName());
    setWindowTitle(fi.baseName());

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("conversation");

    auto items = m_cview->scene()->items();
    QMap<ConversationObject *, int> itemID;
    QMap<int, ConversationObject *> rmap;

    for(auto item : items) {
        auto obj = dynamic_cast<ConversationObject *>(item);
        if(!obj) continue;
        itemID[obj] = obj->id();
        rmap[obj->id()] = obj;
    }

    xml.writeStartElement("objects");
    for(auto obj : rmap) {
        obj->serialize(xml);
    }
    xml.writeEndElement(); // objects

    m_data->serialize(xml);

    xml.writeEndElement(); // </conversation>
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
    m_data->deserialize(doc);

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
        // mark ID as in use...
        m_data->usedIDs().insert(id);

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
    if(mode > SelectMode && mode < SelectOneMode) {
        m_toolButtons[to-1]->setEnabled(false);
    }

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
    case SelectOneMode:
        m_cview->enterSelectMode();
        connect(m_cview, SIGNAL(selected(ConversationObject *)),
            this, SLOT(selectOne(ConversationObject *)));
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
    if(object) {
        object->edit(m_dataInterface, m_data,
            dynamic_cast<QFormLayout *>(m_editarea->layout()));
    }
    if(m_selectLast) m_selectLast->deselect();
    m_selectLast = object;
    if(m_selectLast) m_selectLast->select();

    Node *node = dynamic_cast<Node *>(m_selectLast);
    if(node) m_simbutton->setEnabled(true);
    else m_simbutton->setEnabled(false);
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

void ConversationWindow::selectOne(ConversationObject *object) {
    if(!m_selectOneFilter(object)) return;

    m_selectOneCallback(object);
    
    modeChange(SelectMode);
}

void ConversationWindow::beginSimulation() {
    Node *node = dynamic_cast<Node *>(m_selectLast);
    if(!node) return;

    m_sim->beginFrom(node);
}
