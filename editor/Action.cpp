#include <QStandardItem>
#include <QXmlStreamWriter>
#include <QDomElement>

#include "Action.h"
#include "Node.h"

void Action::updateTitle(QStandardItem *item) {
    QString title;
    switch((ActionType)item->data(TypeData).toInt()) {
    case Empty:
        title = "[empty]";
        break;
    case Speech: {
        QString speaker = item->data(ActorData).toString();
        QString speech = item->data(SpeechData).toString();
        title = "[speech] " + speaker + ": " + speech;
        break;
    }
    case Emote: {
        QString actor = item->data(ActorData).toString();
        QString emote = item->data(EmoteData).toString();
        title = "[emote] * " + actor + " " + emote;
        break;
    }
    case Sequence:
        title = "[sequence]";
        break;
    case Concurrent:
        title = "[concurrent]";
        break;
    case Conditional:
        title = "[conditional]";
        break;
    case Jump: {
        title = "[jump]";
        Node *target = (Node *)item->data(JumpTargetData).value<void *>();
        if(!target) title += "<no target>";
        else title += " " + target->label();
        break;
    }
    case EndConversation:
        title = "[end]";
        break;
    case FirstVisitConditional: {
        bool invert = item->data(ConditionalInversionData).toBool();
        if(!invert) title = "[first-visit?]";
        else title = "[not-first-visit?]";
        break;
    }
    }
    item->setData(title, Qt::DisplayRole);
}

void Action::serialize(QXmlStreamWriter &xml, QStandardItem *action) {
    xml.writeStartElement("action");

    xml.writeAttribute("type",
        action->data(Action::TypeData).toString());

    auto s = action->data(Action::ActorData).toString();
    if(!s.isEmpty()) xml.writeAttribute("actor", s);
    s = action->data(Action::SpeechData).toString();
    if(!s.isEmpty()) xml.writeAttribute("speech", s);
    s = action->data(Action::EmoteData).toString();
    if(!s.isEmpty()) xml.writeAttribute("emote", s);
    s = action->data(Action::CommentData).toString();
    if(!s.isEmpty()) xml.writeAttribute("comment", s);
    auto a = action->data(Action::ConditionalInversionData);
    if(a.isValid() && a.toBool()) xml.writeAttribute("cond-invert", "true");

    auto target = (ConversationObject *)action->data(
        Action::JumpTargetData).value<void *>();
    if(target)
        xml.writeAttribute("jump-target", QString().setNum(target->id()));

    for(int i = 0; i < action->rowCount(); i ++) {
        serialize(xml, action->child(i));
    }

    xml.writeEndElement();
}

QStandardItem *Action::deserialize(QDomElement &xml, 
    const QMap<int, ConversationObject *> &objs) {

    auto action = new QStandardItem();

    action->setData(xml.attribute("type").toInt(), Action::TypeData);
    // XXX: for compatability with old editors, remove after a while
    if(xml.attribute("speaker").length() != 0)
        action->setData(xml.attribute("speaker"), Action::ActorData);
    else action->setData(xml.attribute("actor"), Action::ActorData);

    action->setData(xml.attribute("speech"), Action::SpeechData);
    action->setData(xml.attribute("emote"), Action::EmoteData);
    action->setData(xml.attribute("comment"), Action::CommentData);
    action->setData(xml.attribute("cond-invert") == "true",
        Action::ConditionalInversionData);
    int jtid = xml.attribute("jump-target").toInt();
    if(jtid != 0) {
        action->setData(qVariantFromValue((void *)objs[jtid]),
            Action::JumpTargetData);
    }
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
