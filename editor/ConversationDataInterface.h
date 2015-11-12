#ifndef ConversationDataInterface_H
#define ConversationDataInterface_H

#include <functional>

class ConversationObject;

class ConversationDataInterface {
public:
    virtual ~ConversationDataInterface() {}

    // takes a filter function which returns true if the given object should be
    // passed onto callback, and callback should return true if the selection
    // process is finished.
    virtual void selectObjects(
        std::function<bool (ConversationObject *)> filter,
        std::function<bool (ConversationObject *)> callback) = 0;
};

#endif
