#ifndef ConversationDataInterface_H
#define ConversationDataInterface_H

#include <functional>

class ConversationObject;

class ConversationDataInterface {
public:
    virtual ~ConversationDataInterface() {}

    virtual void selectObject(
        std::function<bool (ConversationObject *)> filter,
        std::function<void (ConversationObject *)> callback) = 0;
};

#endif
