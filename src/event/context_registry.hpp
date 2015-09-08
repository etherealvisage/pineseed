#ifndef EVENT_CONTEXT_REGISTRY_HPP
#define EVENT_CONTEXT_REGISTRY_HPP

#include <vector>

#include "context.hpp"

namespace event {

class context_registry {
private:
    std::vector<context *> m_contexts;
public:
    void push_context(context *context);
    void pop_context();
    
    context *get_top_context();

    void fire_queued();
};

} // namespace event

#endif
