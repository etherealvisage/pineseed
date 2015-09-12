#include <sstream>
#include <iterator>
#include <vector>

#include "lua.hpp"

#include "message_system.hpp"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
} // extern "C"

namespace api {

lua_wrapper::lua_wrapper() {
    m_state = luaL_newstate();

    // add standard libraries
    luaL_openlibs(m_state);

    // metatable for C++ function wrappers
    luaL_newmetatable(m_state, "cpp_wrapper_table");
    lua_pushcfunction(m_state, call_function);
    lua_setfield(m_state, -2, "__call");
    lua_pushcfunction(m_state, gc_function);
    lua_setfield(m_state, -2, "__gc");
    lua_pop(m_state, 1);
}

lua_wrapper::~lua_wrapper() {
    lua_close(m_state);
}

void lua_wrapper::add_api_function(std::string name,
    const wrapped_function_t &function) {
    
    auto *data = lua_newuserdata(m_state, sizeof(function));
    new(data) wrapped_function_t(function);

    luaL_getmetatable(m_state, "cpp_wrapper_table");
    lua_setmetatable(m_state, -2);

    set_global(name);
    lua_pop(m_state, 1);
}

void lua_wrapper::get_table(std::string name) {
    std::stringstream ss(name);
    std::string tname;

    bool first = true;
    while(std::getline(ss, tname, ',')) {
        if(first) {
            first = false;
            lua_getglobal(m_state, tname.c_str());
        }
        else {
            lua_pushstring(m_state, tname.c_str());
            lua_rawget(m_state, -1);
            if(lua_isnil(m_state, -1)) {
                lua_pop(m_state, 1);
                // Stack: name, table
                lua_pushvalue(m_state, -1);
                // Stack: name, name, table
                lua_newtable(m_state);
                // Stack: new table, name, name, table
                lua_rawset(m_state, -4);
                // Stack: name, table
                lua_rawget(m_state, -2);
                // Stack: new table, table
            }
            lua_remove(m_state, -2);
        }
    }
}

void lua_wrapper::set_global(std::string name) {
    // assumes there is something on the stack to use...

    // Extract names
    std::stringstream ss(name);
    std::string tname;
    std::vector<std::string> names;
    while(std::getline(ss, tname, ',')) names.push_back(tname);

    // is this a simple global?
    if(names.size() == 1) {
        lua_setglobal(m_state, name.c_str());
        return;
    }

    lua_getglobal(m_state, names.front().c_str());

    for(decltype(names)::size_type i = 1; i < names.size()-1; i ++) {
        auto &n = names[i];
        lua_getfield(m_state, -1, n.c_str());
        if(lua_isnil(m_state, -1)) {
            lua_pop(m_state, 1);
            lua_newtable(m_state);
            lua_setfield(m_state, -2, n.c_str());
            lua_getfield(m_state, -1, n.c_str());
        }
        lua_remove(m_state, -2);
    }

    lua_setfield(m_state, -1, names.back().c_str());
}

int lua_wrapper::call_function(lua_State *state) {
    wrapped_function_t *function =
        static_cast<wrapped_function_t *>(lua_touserdata(state, 1));
    lua_remove(state, 1);
    return (*function)(state);
}

int lua_wrapper::gc_function(lua_State *state) {
    wrapped_function_t *function = 
        static_cast<wrapped_function_t *>(lua_touserdata(state, 1));
    function->~wrapped_function_t();
    return 0;
}

} // namespace api
