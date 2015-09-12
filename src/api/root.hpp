#ifndef API_ROOT_HPP
#define API_ROOT_HPP

#include "interface.hpp"
#include "lua.hpp"

namespace api {

class root {
private:
    lua_wrapper m_wrapper;
    interface m_interface;
public:
    root();
    ~root();

    lua_wrapper *get_wrapper() { return &m_wrapper; }

    interface *get_interface() { return &m_interface; }
    const interface *get_interface() const { return &m_interface; }
    // const game *get_game() etc.
};

} // namespace api

#endif
