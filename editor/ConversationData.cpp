#include <QDialog>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>
#include <QInputDialog>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QDomElement>

#include "ConversationData.h"

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

    dialog->exec();
    dialog->deleteLater();

    // Character tab
    m_characterNames.clear();
    while(namesList->count() > 0) {
        m_characterNames.push_back(namesList->takeItem(0)->text());
    }
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
}
