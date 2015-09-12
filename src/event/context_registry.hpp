#ifndef EVENT_CONTEXT_REGISTRY_HPP
#define EVENT_CONTEXT_REGISTRY_HPP

#include <vector>

#include "context.hpp"

namespace event {

class context_registry {
private:
    std::vector<base_context *> m_contexts;
public:
    void push_context(base_context *context);
    void pop_context();
    
    base_context *get_top_context();

    void fire_queued();
};

} // namespace event

#endif
