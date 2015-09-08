#ifndef INTERFACE_EVENT_HPP
#define INTERFACE_EVENT_HPP

#include <functional>
#include <map>

#include <SDL/SDL_events.h>

namespace interface {

class event {
public:
    typedef std::function<void (SDL_Event *)> listener;

    std::multimap<int, listener> m_listeners;
public:
    void register_listener(int type, listener listener)
        { m_listeners.insert(std::make_pair(type, listener)); }

    void poll_events();
};

} // namespace interface

#endif
