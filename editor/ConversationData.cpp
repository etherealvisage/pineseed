#include <QDialog>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>
#include <QInputDialog>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QDomElement>
#include <QTreeWidget>
#include <QStandardItem>
#include <QStandardItemModel>

#include "ConversationData.h"
#include "ConversationContext.h"

ConversationContext *ConversationData::rootContext() {
    if(m_rootContext == nullptr) {
        m_rootContext = new ConversationContext(getAvailableID());
        m_contexts[m_rootContext->id()] = m_rootContext;
    }

    return m_rootContext;
}

QPointer<ConversationContext> ConversationData::getContextByID(int id) {
    return m_contexts[id];
}

QPointer<ConversationContext> ConversationData::selectContext(
    QWidget *parent) {

    qDebug("Context selection NYI");

    return QPointer<ConversationContext>();
}

int ConversationData::getAvailableID() {
    int ret = -1;
    do {
        ret = qrand();
    } while(m_usedIDs.contains(ret));

    m_usedIDs.insert(ret);
    return ret;
}

void ConversationData::edit(QWidget *parent) {
    QDialog *dialog = new QDialog(parent);
    QVBoxLayout *primaryLayout = new QVBoxLayout();

    QTabWidget *tabs = new QTabWidget();
    primaryLayout->addWidget(tabs);

    QPushButton *closeButton = new QPushButton(QObject::tr("&Close"));
    QObject::connect(closeButton, SIGNAL(clicked(bool)),
        dialog, SLOT(accept()));
    primaryLayout->addWidget(closeButton);

    dialog->setLayout(primaryLayout);

    // Character tab
    QVBoxLayout *namesLayout = new QVBoxLayout();
    QListWidget *namesList = new QListWidget;
    for(auto &name : m_characterNames) {
        namesList->addItem(name);
    }
    namesLayout->addWidget(namesList);

    QPushButton *addName = new QPushButton(QObject::tr("&Add"));
    namesLayout->addWidget(addName);
    QObject::connect(addName, &QPushButton::clicked, 
        [=](){
            bool ok = true;
            QString name = QInputDialog::getText(dialog,
                QObject::tr("New name"),
                QObject::tr("Enter new character name:"), QLineEdit::Normal,
                "", &ok);
            if(ok) namesList->addItem(name);
            else namesList->addItem("????");
        });
    QPushButton *removeName = new QPushButton(QObject::tr("&Delete"));
    QObject::connect(removeName, &QPushButton::clicked, 
        [=](){
            for(auto it : namesList->selectedItems()) {
                delete it;
            }
        });
    namesLayout->addWidget(removeName);

    QWidget *namesWidget = new QWidget();
    namesWidget->setLayout(namesLayout);
    tabs->addTab(namesWidget, QObject::tr("Character names"));

    // Contexts tab
    auto contextsLayout = new QVBoxLayout();
    auto contextsModel = new QStandardItemModel();
    auto contextsView = new QTreeView();
    contextsView->setModel(contextsModel);
    contextsView->setDragDropMode(QAbstractItemView::InternalMove);

    auto rootItem = new QStandardItem();
    QMap<int, QStandardItem *> contextItems;
    rootItem->setText("Root context");
    rootItem->setEditable(false);
    rootItem->setDragEnabled(false);
    rootItem->setDropEnabled(true);
    rootItem->setData(rootContext()->id(), 0x100);
    contextsModel->invisibleRootItem()->appendRow(rootItem);
    contextItems[rootContext()->id()] = rootItem;
    contextsLayout->addWidget(contextsView);

    for(auto con : m_contexts) {
        if(contextItems.contains(con->id())) continue;
        auto item = new QStandardItem();

        item->setData(con->id(), 0x100);
        item->setText(con->label());
        item->setEditable(true);
        item->setDragEnabled(true);
        item->setDropEnabled(true);

        contextItems[con->id()] = item;
    }
    for(auto con : m_contexts) {
        if(!con->parent()) continue;
        contextItems[con->parent()->id()]->appendRow(contextItems[con->id()]);
    }

    auto addContext = new QPushButton(QObject::tr("&Add"));
    QObject::connect(addContext, &QPushButton::clicked, 
        [=](){
            bool ok = true;
            QString name = QInputDialog::getText(dialog,
                QObject::tr("New context name"),
                QObject::tr("Enter new context name:"), QLineEdit::Normal,
                "", &ok);
            if(!ok) name = "????";
            int id = getAvailableID();
            m_contexts[id] = new ConversationContext(id);
            auto item = new QStandardItem();

            item->setData(id, 0x100);
            item->setText(name);
            item->setEditable(true);
            item->setDragEnabled(true);
            item->setDropEnabled(true);
            rootItem->appendRow(item);
        });
    contextsLayout->addWidget(addContext);
    auto removeContext = new QPushButton(QObject::tr("&Delete"));
    QObject::connect(removeContext, &QPushButton::clicked, 
        [=](){
            qDebug("Removal NYI");
        });
    contextsLayout->addWidget(removeContext);

    auto contextsWidget = new QWidget();
    contextsWidget->setLayout(contextsLayout);
    tabs->addTab(contextsWidget, QObject::tr("Node contexts"));

    dialog->exec();

    // Character tab
    m_characterNames.clear();
    while(namesList->count() > 0) {
        m_characterNames.push_back(namesList->takeItem(0)->text());
    }

    // Contexts tab
    // XXX: local function replacement
    class ContextsRearranger {
    public:
        static void rearrange(QMap<int, ConversationContext *> &contexts,
            QStandardItem *item) {

            auto thisID = item->data(0x100).toInt();
            auto thisContext = contexts[thisID];
            for(int i = 0; i < item->rowCount(); i ++) {
                auto it = item->child(i);
                auto id = it->data(0x100).toInt();
                contexts[id]->setParent(thisContext);
                contexts[id]->setLabel(it->text());
                rearrange(contexts, item->child(i));
            }
        }
    };

    ContextsRearranger::rearrange(m_contexts, rootItem);

    // TODO: delete all unparented contexts

    dialog->deleteLater();
}

void ConversationData::serialize(QXmlStreamWriter &xml) {
    xml.writeStartElement("data");

    xml.writeStartElement("character-names");

    for(auto n : m_characterNames) {
        xml.writeStartElement("character");
        xml.writeAttribute("name", n);
        xml.writeEndElement(); // </character>
    }

    xml.writeEndElement(); // </character-names>

    xml.writeStartElement("contexts");

    xml.writeAttribute("root", QString().setNum(rootContext()->id()));

    for(auto n : m_contexts) {
        xml.writeStartElement("context");
        xml.writeAttribute("id", QString().setNum(n->id()));
        xml.writeAttribute("label", n->label());
        if(!n->parent().isNull())
            xml.writeAttribute("parent", QString().setNum(n->parent()->id()));
        xml.writeEndElement();
    }

    xml.writeEndElement(); // </contexts>

    xml.writeEndElement(); // </data>
}

void ConversationData::deserialize(QDomDocument &doc) {
    auto datas = doc.elementsByTagName("data");
    if(datas.length() != 1) {
        qDebug("Expected exactly one <data> element! Found %d.", datas.length());
        return;
    }
    auto data = datas.at(0);
    auto cnamess = data.toElement().elementsByTagName("character-names");
    if(cnamess.length() != 1) {
        qDebug("Expected exactly one <character-names> element!");
        return;
    }
    auto cnames = cnamess.at(0);
    auto nodes = cnames.childNodes();
    for(int i = 0; i < nodes.length(); i ++) {
        auto name = nodes.at(i);
        m_characterNames.push_back(
            name.toElement().attribute("name", "???"));
    }

    auto contextss = data.toElement().elementsByTagName("contexts");
    if(contextss.length() != 1) {
        qDebug("Expected exactly one <contexts> element!");
        return;
    }
    auto contexts = contextss.at(0);
    nodes = contexts.childNodes();
    // first pass to construct everything
    for(int i = 0; i < nodes.length(); i ++) {
        auto context = nodes.at(i);
        int id = context.toElement().attribute("id").toInt();
        QString label = context.toElement().attribute("label");

        auto con = m_contexts[id] = new ConversationContext(id);
        con->setLabel(label);
    }
    // second pass to set parents
    for(int i = 0; i < nodes.length(); i ++) {
        auto context = nodes.at(i);
        int id = context.toElement().attribute("id").toInt();
        bool hasParent = false;
        int parentID =
            context.toElement().attribute("parent").toInt(&hasParent);
        if(hasParent) m_contexts[id]->setParent(m_contexts[parentID]);
    }

    m_rootContext = m_contexts[contexts.toElement().attribute("root").toInt()];
}
