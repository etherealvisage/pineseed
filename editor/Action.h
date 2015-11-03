#ifndef Action_H
#define Action_H

#include <QMap>

class QString;
class QStandardItem;
class QDomElement;
class QXmlStreamWriter;
class ConversationObject;

#define ActionList \
    Action(Empty),                  \
    Action(Speech),                 \
    Action(Emote),                  \
    Action(Sequence),               \
    Action(Concurrent),             \
    Action(Conditional),            \
    Action(Jump),                   \
    Action(EndConversation),        \
    Action(FirstVisitConditional)

class Action {
public:
    // make sure this is synchronized with the ActionTypeNames in Action.cpp
    enum ActionType {
    #define Action(n) n
        ActionList
    #undef Action
        , ActionTypes
    };

    enum ItemData {
        TypeData = 0x100,
        ActorData,
        SpeechData,
        EmoteData,
        JumpTargetData,
        ConditionalInversionData,
        CommentData
    };
public:
    static ActionType typeFromName(const QString &name);
    static ActionType typeFromName(const char *name);
    static const char *nameFromType(ActionType type);

    static void updateTitle(QStandardItem *item);

    static void serialize(QXmlStreamWriter &xml, QStandardItem *action);
    static QStandardItem *deserialize(QDomElement &xml, 
        const QMap<int, ConversationObject *> &objs);
};

#endif
