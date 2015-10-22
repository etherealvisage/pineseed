#ifndef Action_H
#define Action_H

#include <QString>

#include "ConversationObject.h"

class QWidget;

class Action : public ConversationObject {
public:
    enum ActionType {
        SpeechAction,
    };
private:
    ActionType m_type;
    QString m_character;
private:
    Action() {}
public:
    static Action *createAction(QWidget *parent);

    QString description() const;

    virtual void edit(QGraphicsView *view);
};

#endif
