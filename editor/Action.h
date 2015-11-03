#ifndef Action_H
#define Action_H

#include <QMap>

class QStandardItem;
class QDomElement;
class QXmlStreamWriter;
class ConversationObject;

class Action {
public:
    enum ActionType {
        Empty,
        Speech,
        Emote,
        Sequence,
        Concurrent,
        Conditional,
        FirstVisitConditional,
        Jump,
        EndConversation
    };
    enum ItemData {
        TypeData = 0x100,
        ActorData,
        SpeechData,
        EmoteData,
        JumpTargetData,
        CommentData
    };
public:
    static void updateTitle(QStandardItem *item);

    static void serialize(QXmlStreamWriter &xml, QStandardItem *action);
    static QStandardItem *deserialize(QDomElement &xml, 
        const QMap<int, ConversationObject *> &objs);
};

#endif
