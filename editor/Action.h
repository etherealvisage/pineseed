#ifndef Action_H
#define Action_H

#include <QString>

class QWidget;

class Action {
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

    void edit(QWidget *parent);
};

#endif
