#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSignalMapper>
#include <QShortcut>
#include <QFileInfo>
#include <QXmlStreamWriter>
#include <QGraphicsItem>
#include <QList>
#include <QFormLayout>
#include <QFileDialog>
#include <QDomDocument>

#include "PlatformWindow.h"
#include "moc_PlatformWindow.cpp"

#include "Platform.h"
#include "EditorView.h"
#include "PlatformData.h"

PlatformWindow::PlatformWindow() : m_selectLast(nullptr) {
    m_data = new PlatformData();

    auto split = new QSplitter();
    setWidget(split);

    QWidget *editWidget = new QWidget();
    split->addWidget(editWidget);
    auto editLayout = new QVBoxLayout();
    editWidget->setLayout(editLayout);

    auto modeLayout = new QHBoxLayout();
    editLayout->addLayout(modeLayout);
    editLayout->addStretch(1);

    m_editarea = new QWidget();
    editLayout->addWidget(m_editarea);
    m_editarea->setLayout(new QFormLayout());

    m_modeMapper = new QSignalMapper(this);
    connect(m_modeMapper, SIGNAL(mapped(int)), this, SLOT(modeChange(int)));
    {
        auto selectPlatform = new QPushButton(tr("Select"));
        m_toolButtons.push_back(selectPlatform);
        modeLayout->addWidget(selectPlatform);
        m_modeMapper->setMapping(selectPlatform, SelectMode);
        connect(selectPlatform, SIGNAL(clicked(bool)), m_modeMapper,
            SLOT(map()));
        QShortcut *selectPlatformShortcut =
            new QShortcut(QKeySequence("Alt+1"), this);
        connect(selectPlatformShortcut, SIGNAL(activated()),
            selectPlatform, SLOT(click()));
    }
    {
        auto newPlatform = new QPushButton(tr("Add"));
        m_toolButtons.push_back(newPlatform);
        modeLayout->addWidget(newPlatform);
        m_modeMapper->setMapping(newPlatform, NewPlatformMode);
        connect(newPlatform, SIGNAL(clicked(bool)), m_modeMapper, SLOT(map()));
        QShortcut *newPlatformShortcut =
            new QShortcut(QKeySequence("Alt+2"), this);
        connect(newPlatformShortcut, SIGNAL(activated()),
            newPlatform, SLOT(click()));
    }
    {
        auto removePlatform = new QPushButton(tr("Remove"));
        m_toolButtons.push_back(removePlatform);
        modeLayout->addWidget(removePlatform);
        m_modeMapper->setMapping(removePlatform, DeleteMode);
        connect(removePlatform, SIGNAL(clicked(bool)),
            m_modeMapper, SLOT(map()));
        QShortcut *removePlatformShortcut =
            new QShortcut(QKeySequence("Alt+3"), this);
        connect(removePlatformShortcut, SIGNAL(activated()),
            removePlatform, SLOT(click()));
    }

    m_eview = new EditorView();
    split->addWidget(m_eview);
    modeChange(SelectMode);
}

void PlatformWindow::saveTo(QFile &file) {
    QFileInfo fi(file.fileName());
    setWindowTitle(fi.baseName());

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("platform");

    auto items = m_eview->scene()->items();
    QMap<PlatformObject *, int> itemID;
    QMap<int, PlatformObject *> rmap;

    for(auto item : items) {
        auto obj = dynamic_cast<PlatformObject *>(item);
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

void PlatformWindow::load() {
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open platform"));
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

    QMap<int, PlatformObject *> objs;
    auto nodes = objects.childNodes();
    for(int i = 0; i < nodes.length(); i ++) {
        auto element = nodes.at(i).toElement();

        auto name = element.tagName();
        auto id = element.attribute("id").toInt();
        // mark ID as in use...
        m_data->usedIDs().insert(id);

        if(name == "platform") {
            objs[id] = new Platform();
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

void PlatformWindow::modeChange(int to) {
    m_eview->disconnect(m_eview, SIGNAL(clicked(QPointF)), this, 0);
    m_eview->disconnect(m_eview, SIGNAL(selected(EditorObject *)),
        this, 0);

    for(auto b : m_toolButtons) if(!b->isEnabled())
        b->setEnabled(true), b->setStyleSheet("");
    m_toolButtons[to]->setEnabled(false);
    m_toolButtons[to]->setStyleSheet("background-color: red");

    switch((Mode)to) {
    case SelectMode:
        m_eview->enterSelectMode();
        connect(m_eview, SIGNAL(selected(EditorObject *)),
            this, SLOT(selectObject(EditorObject *)));
        break;
    case NewPlatformMode:
        m_eview->enterInsertMode();
        connect(m_eview, SIGNAL(clicked(QPointF)),
            this, SLOT(insertPlatform(QPointF)));
        break;
    case DeleteMode:
        m_eview->enterSelectMode();
        connect(m_eview, SIGNAL(selected(EditorObject *)),
            this, SLOT(deleteObject(EditorObject *)));
        break;
    }
}

void PlatformWindow::selectObject(EditorObject *eobject) {
    auto object = dynamic_cast<PlatformObject *>(eobject);
    while(m_editarea->layout()->count() > 0) {
        auto item = m_editarea->layout()->takeAt(0);
        if(item->widget()) item->widget()->deleteLater();
        if(item->layout()) delete item->layout();

        delete item;
    }

    if(object) {
        object->edit(m_data,
            dynamic_cast<QFormLayout *>(m_editarea->layout()));
    }

    if(m_selectLast) m_selectLast->deselect();
    m_selectLast = object;
    if(m_selectLast) m_selectLast->select();
}

void PlatformWindow::insertPlatform(QPointF where) {
    QRectF rect(0, 0, 100, 100);
    rect.moveCenter(where);
    auto platform = new Platform(rect);
    m_eview->scene()->addItem(platform);

    connect(platform, SIGNAL(changed()), m_eview->viewport(), SLOT(update()));
    selectObject(platform);
    modeChange(SelectMode);
}

void PlatformWindow::deleteObject(EditorObject *object) {
    object->deleteLater();

    modeChange(SelectMode);
    selectObject(nullptr);
}
