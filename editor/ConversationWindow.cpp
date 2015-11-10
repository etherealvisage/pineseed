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
#include <QMessageBox>
#include <QIcon>

#include "ConversationWindow.h"
#include "moc_ConversationWindow.cpp"

#include "Node.h"
#include "Link.h"
#include "ActionEditor.h" // for ItemData definitions
#include "ConversationSimulation.h"
#include "ConversationDataInterface.h"
#include "Context.h"

ConversationWindow::ConversationWindow() {
    m_data = new ConversationData();

    auto split = new QSplitter();
    m_edit = new QWidget();
    split->addWidget(m_edit);
    m_eview = new EditorView();
    split->addWidget(m_eview);
    m_sim = new ConversationSimulation();
    split->addWidget(m_sim);
    setWidget(split);

    QVBoxLayout *editLayout = new QVBoxLayout();

    m_modeMapper = new QSignalMapper();
    connect(m_modeMapper, SIGNAL(mapped(int)), this, SLOT(modeChange(int)));
    QHBoxLayout *editbarLayout = new QHBoxLayout();

    QPushButton *selectButton =
        new QPushButton(QIcon(":/icons/select.png"), "");
    connect(selectButton, SIGNAL(clicked(bool)), m_modeMapper, SLOT(map()));
    m_modeMapper->setMapping(selectButton, SelectMode);
    editbarLayout->addWidget(selectButton);
    m_toolButtons.push_back(selectButton);
    QShortcut *selectShortcut = new QShortcut(QKeySequence("Alt+0"), this);
    connect(selectShortcut, SIGNAL(activated()), selectButton, SLOT(click()));

    QPushButton *newNodeButton =
        new QPushButton(QIcon(":/icons/node.png"), "");
    connect(newNodeButton, SIGNAL(clicked(bool)), m_modeMapper, SLOT(map()));
    m_modeMapper->setMapping(newNodeButton, NewNodeMode);
    editbarLayout->addWidget(newNodeButton);
    m_toolButtons.push_back(newNodeButton);
    QShortcut *newNodeShortcut = new QShortcut(QKeySequence("Alt+1"), this);
    connect(newNodeShortcut, SIGNAL(activated()),
        newNodeButton, SLOT(click()));

    QPushButton *newLinkButton = 
        new QPushButton(QIcon(":/icons/link.png"), "");
    connect(newLinkButton, SIGNAL(clicked(bool)), m_modeMapper, SLOT(map()));
    m_modeMapper->setMapping(newLinkButton, NewLinkMode);
    editbarLayout->addWidget(newLinkButton);
    m_toolButtons.push_back(newLinkButton);
    QShortcut *newLinkShortcut = new QShortcut(QKeySequence("Alt+2"), this);
    connect(newLinkShortcut, SIGNAL(activated()),
        newLinkButton, SLOT(click()));

    QPushButton *newContextButton =
        new QPushButton(QIcon(":/icons/context.png"), "");
    connect(newContextButton, SIGNAL(clicked(bool)), m_modeMapper,
        SLOT(map()));
    m_modeMapper->setMapping(newContextButton, NewContextMode);
    editbarLayout->addWidget(newContextButton);
    m_toolButtons.push_back(newContextButton);
    QShortcut *newContextShortcut = new QShortcut(QKeySequence("Alt+3"), this);
    connect(newContextShortcut, SIGNAL(activated()),
        newContextButton, SLOT(click()));

    QPushButton *deleteButton =
        new QPushButton(QIcon(":/icons/delete.png"), "");
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
    //m_edit->setFixedSize(QSize(300, 300));
    //m_edit->setMinimumSize(QSize(300, 200));
    m_edit->setMinimumWidth(300);
    m_edit->setMaximumWidth(300);
    //m_edit->setMaximumSize(QSize(300, 200));
    m_edit->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,
        QSizePolicy::MinimumExpanding));

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

    modeChange(SelectMode); // enter select mode by default
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

    auto items = m_eview->scene()->items();
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
        else if(name == "context-ref") {
            objs[id] = new Context();
        }
        else {
            qDebug("Unknown object type %s", name.toLocal8Bit().constData());
        }
    }

    for(int i = 0; i < nodes.length(); i ++) {
        auto element = nodes.at(i).toElement();

        auto id = element.attribute("id").toInt();
        
        objs[id]->deserialize(element, objs, m_data);
        m_eview->scene()->addItem(objs[id]);
    }
}

void ConversationWindow::doWordCount() {
    int wordCount = 0;
    std::function<void (QStandardItem *)> visitor =
        [&wordCount](QStandardItem *item){
            QString data;
            if(item->data(Action::TypeData).toInt() == Action::Speech) {
                data = item->data(Action::SpeechData).toString();
            }
            else if(item->data(Action::TypeData).toInt() == Action::Emote) {
                data = item->data(Action::EmoteData).toString();
            }

            wordCount += data.split(QRegExp("\\s"),
                QString::SkipEmptyParts).length();
        };

    for(auto item : m_eview->items()) {
        auto node = dynamic_cast<Node *>(item);
        if(node) node->visitActions(visitor);
    }

    QMessageBox::information(this, tr("Word count"),
        QString("Speech and emote word count: ")
            + QString().setNum(wordCount));
}

void ConversationWindow::modeChange(int to) {
    Mode mode = (Mode)to;

    m_eview->disconnect(m_eview, SIGNAL(clicked(QPointF)), this, 0);
    m_eview->disconnect(m_eview, SIGNAL(selected(EditorObject *)),
        this, 0);

    for(auto b : m_toolButtons) if(!b->isEnabled()) b->setEnabled(true);
    if(mode < SelectOneMode) {
        m_toolButtons[to]->setEnabled(false);
    }

    switch(mode) {
    case SelectMode:
        m_eview->enterSelectMode();
        connect(m_eview, SIGNAL(selected(EditorObject *)),
            this, SLOT(selectObject(EditorObject *)));
        break;
    case NewNodeMode:
        m_eview->enterInsertMode();
        connect(m_eview, SIGNAL(clicked(QPointF)),
            this, SLOT(insertNode(QPointF)));
        break;
    case NewContextMode:
        m_eview->enterInsertMode();
        connect(m_eview, SIGNAL(clicked(QPointF)),
            this, SLOT(insertContext(QPointF)));
        break;
    case NewLinkMode:
        if(!m_selectLast || !dynamic_cast<LinkableObject *>(m_selectLast)) {
            modeChange(SelectMode);
        }
        else {
            m_eview->enterSelectMode();
            connect(m_eview, SIGNAL(selected(EditorObject *)),
                this, SLOT(makeLink(EditorObject *)));
        }
        break;
    case DeleteMode:
        m_eview->enterSelectMode(Qt::ForbiddenCursor);
        connect(m_eview, SIGNAL(selected(EditorObject *)),
            this, SLOT(deleteObject(EditorObject *)));
        break;
    case SelectOneMode:
        m_eview->enterSelectMode();
        connect(m_eview, SIGNAL(selected(EditorObject *)),
            this, SLOT(selectOne(EditorObject *)));
        break;
    default:
        qFatal("Unexpected mode change value");
        break;
    }
}

void ConversationWindow::selectObject(EditorObject *eobject) {
    auto object = dynamic_cast<ConversationObject *>(eobject);
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
    m_eview->scene()->addItem(node);

    connect(node, SIGNAL(changed()), m_eview->viewport(), SLOT(update()));
    selectObject(node);
    modeChange(SelectMode);
}

void ConversationWindow::insertContext(QPointF where) {
    Context *context = new Context();
    context->setPos(where - context->boundingRect().center());
    m_eview->scene()->addItem(context);

    connect(context, SIGNAL(changed()), m_eview->viewport(), SLOT(update()));
    selectObject(context);
    modeChange(SelectMode);
}

void ConversationWindow::makeLink(EditorObject *object) {
    Node *target = dynamic_cast<Node *>(object);
    // not allowed to have self-links for now
    if(!target || target == m_selectLast) return; 

    Link *link =
        new Link(dynamic_cast<LinkableObject *>(m_selectLast), target);
    connect(link, SIGNAL(changed()), m_eview->viewport(), SLOT(update()));
    m_eview->scene()->addItem(link);

    selectObject(link);
    modeChange(SelectMode);
}

void ConversationWindow::deleteObject(EditorObject *object) {
    object->deleteLater();

    modeChange(SelectMode);
    selectObject(nullptr);
}

void ConversationWindow::selectOne(EditorObject *eobject) {
    auto object = dynamic_cast<ConversationObject *>(eobject);
    if(!m_selectOneFilter(object)) return;

    m_selectOneCallback(object);
    
    modeChange(SelectMode);
}

void ConversationWindow::beginSimulation() {
    Node *node = dynamic_cast<Node *>(m_selectLast);
    if(!node) return;

    m_sim->beginFrom(node);
}
