#include <QStandardItem>
#include <QXmlStreamWriter>
#include <QDomElement>

#include "Action.h"

void Action::updateTitle(QStandardItem *item) {
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

void Action::serialize(QXmlStreamWriter &xml, 
    const QMap<ConversationObject *, int> &itemID, QStandardItem *action) {

    xml.writeStartElement("action");

    xml.writeAttribute("type",
        action->data(Action::TypeData).toString());
    xml.writeAttribute("speech",
        action->data(Action::SpeechData).toString());

    for(int i = 0; i < action->rowCount(); i ++) {
        serialize(xml, itemID, action->child(i));
    }

    xml.writeEndElement();
}

QStandardItem *Action::deserialize(QDomElement &xml, 
        const QMap<int, ConversationObject *> &objs) {

    auto action = new QStandardItem();

    action->setData(xml.attribute("type").toInt(), Action::TypeData);
    action->setData(xml.attribute("speech"), Action::SpeechData);
    Action::updateTitle(action);

    auto nodes = xml.childNodes();
    for(int i = 0; i < nodes.size(); i ++) {
        auto node = nodes.at(i);
        if(!node.isElement()) continue;
        auto element = node.toElement();
        if(element.tagName() != "action") continue;

        action->appendRow(deserialize(element, objs));
    }

    return action;
}
