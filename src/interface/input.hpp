#ifndef INTERFACE_INPUT_HPP
#define INTERFACE_INPUT_HPP

#include <map>
#include <functional>

#include <SDL/SDL.h>

#include "event.hpp"

namespace interface {

class input {
public:
    typedef std::function<bool (SDLKey, bool)> listener;

    std::multimap<SDLKey, listener> m_listeners;
public:
    void setup(event *event);

    void listen(SDLKey key, listener listener);
private:
    void key_pressed(SDL_Event *event);
    void key_released(SDL_Event *event);
};

} // namespace interface

#endif
