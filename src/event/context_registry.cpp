#include "context_registry.hpp"

#include "message_system.hpp"

namespace event {

void context_registry::push_context(base_context *context) {
    m_contexts.push_back(context);
}

void context_registry::pop_context() {
    if(m_contexts.empty()) {
        MSG3(event, error, "No top context to pop!");
    }
    else m_contexts.pop_back();
}

base_context *context_registry::get_top_context() {
    if(m_contexts.empty()) {
        MSG3(event, error, "No top context!");
        return nullptr;
    }
    return m_contexts.back();
}

void context_registry::fire_queued() {
    for(auto context : m_contexts) {
        context->fire_queued();
    }
}

} // namespace event
