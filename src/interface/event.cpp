#include "event.hpp"

namespace interface {

void event::poll_events() {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        auto it = m_listeners.find(e.type);
        while(it != m_listeners.end() && it->first == e.type) {
            it->second(&e);
            it ++;
        }
    }
}

} // namespace interface
