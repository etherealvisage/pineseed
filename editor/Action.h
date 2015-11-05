#ifndef Action_H
#define Action_H

#include <functional>

#include <QMap>

class QString;
class QStandardItem;
class QDomElement;
class QXmlStreamWriter;
class ConversationObject;

#define ActionList \
    Action(Empty, false),               \
    Action(Speech, false),              \
    Action(Emote, false),               \
    Action(Sequence, true),             \
    Action(Concurrent, true),           \
    Action(Conditional, true),          \
    Action(Jump, false),                \
    Action(EndConversation, false),     \
    Action(FirstVisitConditional, true),\
    Action(ReturnToSender, false),      \
    Action(RandomChoice, true)

class Action {
public:
    // make sure this is synchronized with the ActionTypeNames in Action.cpp
    enum ActionType {
    #define Action(n,c) n
        ActionList
    #undef Action
        , ActionTypes
    };

    enum ItemData {
        TypeData = 0x100, // int
        ActorData, // string
        SpeechData, // string
        EmoteData, // string
        JumpTargetData, // void* pointer to QPointer<Node>
        ConditionalInversionData, // bool
        CommentData // string
    };
public:
    static ActionType typeFromName(const QString &name);
    static ActionType typeFromName(const char *name);
    static const char *nameFromType(ActionType type);

    static bool isContainer(ActionType type);
    static bool isContainer(QStandardItem *item);

    static void updateTitle(QStandardItem *item);

    static void serialize(QXmlStreamWriter &xml, QStandardItem *action);
    static QStandardItem *deserialize(QDomElement &xml, 
        const QMap<int, ConversationObject *> &objs);

    static void walkTree(std::function<void (QStandardItem *)> visitor,
        QStandardItem *action);
};

#endif
