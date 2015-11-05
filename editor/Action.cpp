#include <cstring>
#include <cstddef>

#include <QStandardItem>
#include <QXmlStreamWriter>
#include <QDomElement>
#include <QPointer>

#include "Action.h"
#include "Node.h"

const char *ActionTypeNames[Action::ActionTypes+1] = {
#define Action(n,c) #n
    ActionList
#undef Action
    , ""
};

const bool ActionTypeIsContainer[Action::ActionTypes+1] = {
#define Action(n,c) c
    ActionList
#undef Action
    , false
};

bool Action::isContainer(ActionType type) {
    return ActionTypeIsContainer[type];
}

bool Action::isContainer(QStandardItem *item) {
    auto type = item->data(TypeData).toInt();
    return ActionTypeIsContainer[type];
}

Action::ActionType Action::typeFromName(const QString &name) {
    return typeFromName(name.toLocal8Bit().constData());
}

Action::ActionType Action::typeFromName(const char *name) {
    for(int i = 0; i < Action::ActionTypes; i ++) {
        if(!std::strcmp(name, ActionTypeNames[i]))
            return (Action::ActionType)i;
    }
    return Action::ActionTypes;
}

const char *Action::nameFromType(Action::ActionType type) {
    return ActionTypeNames[type];
}

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
        QPointer<Node> *target =
            (QPointer<Node> *)item->data(JumpTargetData).value<void *>();
        if(!target || !target->data()) title += "<no target>";
        else title += " " + (*target)->label();
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
    case ReturnToSender:
        title = "[return]";
        break;
    case ActionTypes:
        qDebug("Invalid action type!");
        break;
    }
    item->setData(title, Qt::DisplayRole);
}

void Action::serialize(QXmlStreamWriter &xml, QStandardItem *action) {
    xml.writeStartElement("action");

    xml.writeAttribute("type", nameFromType(
        (Action::ActionType)action->data(Action::TypeData).toInt()));

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

    auto target = (QPointer<ConversationObject> *)action->data(
        Action::JumpTargetData).value<void *>();
    if(target && target->data()) {
        int targetID = (*target)->id();
        xml.writeAttribute("jump-target", QString().setNum(targetID));
    }

    for(int i = 0; i < action->rowCount(); i ++) {
        serialize(xml, action->child(i));
    }

    xml.writeEndElement();
}

QStandardItem *Action::deserialize(QDomElement &xml, 
    const QMap<int, ConversationObject *> &objs) {

    auto action = new QStandardItem();

    auto typeAttribute = xml.attribute("type");
    bool isIntType = false;
    // XXX: for compatability with old editors, remove after a while
    if(typeAttribute.toInt(&isIntType))
        action->setData(typeAttribute.toInt(), Action::TypeData);
    else 
        action->setData(typeFromName(typeAttribute), Action::TypeData);
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
        action->setData(
            qVariantFromValue((void *)new QPointer<Node>((Node *)objs[jtid])),
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

void Action::walkTree(std::function<void (QStandardItem *)> visitor,
    QStandardItem *action) {

    visitor(action);

    auto type = (ActionType)action->data(TypeData).toInt();
    if(isContainer(type)) {
        for(int i = 0; i < action->rowCount(); i ++) {
            walkTree(visitor, action->child(i));
        }
    }
}
