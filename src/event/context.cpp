#include "context.hpp"

namespace event {

void context::fire_queued() {
    decltype(m_events) events_copy;
    std::swap(events_copy, m_events);

    for(auto &event : m_events) {
        event.second();
    }
}

} // namespace event
