#include "input.hpp"

namespace interface {

void input::setup(event *event) {
    event->register_listener(SDL_KEYDOWN,
        std::bind(&input::key_pressed, this, std::placeholders::_1));
    event->register_listener(SDL_KEYDOWN,
        std::bind(&input::key_released, this, std::placeholders::_1));
}

void input::listen(SDLKey key, listener listener) {
    m_listeners.insert(std::make_pair(key, listener));
}

void input::key_pressed(SDL_Event *event) {
    auto sym = event->key.keysym.sym;
    auto it = m_listeners.find(sym);
    while(it != m_listeners.end() && it->first == sym) {
        it->second(it->first, true);
        it ++;
    }
}

void input::key_released(SDL_Event *event) {
    auto sym = event->key.keysym.sym;
    auto it = m_listeners.find(sym);
    while(it != m_listeners.end() && it->first == sym) {
        it->second(it->first, false);
        it ++;
    }
}

} // namespace interface
