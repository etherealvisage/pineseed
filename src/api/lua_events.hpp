#ifndef API_LUA_EVENTS_HPP
#define API_LUA_EVENTS_HPP

#include <iostream> // debugging

#include "lua.hpp"

#include "event/context.hpp"

namespace api {

namespace hidden {

template<typename visitor_t, int v, typename P, class ...Ts>
class proxy_helper {
public:
    static void proxy(visitor_t &vis, std::string, const P &) {
        vis.done();
    }
};

template<typename visitor_t, int v, typename P, class T, class ...Ts>
class proxy_helper<visitor_t, v, P, T, Ts...> {
public:
    static void proxy(visitor_t &vis, std::string name, const P &params) {
        vis.parameter(std::get<v>(params));
        proxy_helper<visitor_t, v+1, P, Ts...>::proxy(vis, name, params);
    }
};

class visitor_t {
private:
    lua_wrapper *m_wrapper;
    int m_argcount;
public:
    visitor_t(lua_wrapper *wrapper) : m_wrapper(wrapper), m_argcount(0) {}

    int get_argcount() { return m_argcount; }

    template<typename T>
    void parameter(const T &arg) {
        std::cout << __PRETTY_FUNCTION__ << " : value is " << arg << std::endl;
    }

    void parameter(const int &arg) {
        std::cout << "integer!" << std::endl;
        lua_pushinteger(m_wrapper->get_state(), arg);
        m_argcount ++;
    }

    void done() {
        std::cout << "Done!" << std::endl;
    }
};

} // namespace hidden

class lua_event_proxy {
    lua_wrapper *m_wrapper;
public:
    lua_event_proxy(lua_wrapper *wrapper) : m_wrapper(wrapper) {}

    template<typename ...T>
    void proxy(std::string name, const std::tuple<T...> &params) {
        hidden::visitor_t vis(m_wrapper);

        auto state = m_wrapper->get_state();
        lua_getglobal(state, name.c_str());
        if(lua_isnil(state, -1)) {
            lua_pop(state, 1);
            std::cout << "No function named \"" << name << "\"\n";
            return;
        }

        hidden::proxy_helper<
            hidden::visitor_t, 0, decltype(params), T...>::proxy(
                vis, name, params);

        lua_call(state, vis.get_argcount(), 0);
    }
};

using lua_event_context = event::proxy_context<lua_event_proxy>;

} // namespace api

#endif
