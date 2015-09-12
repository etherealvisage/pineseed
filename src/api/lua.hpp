#ifndef API_LUA_HPP
#define API_LUA_HPP

#include <functional>
#include <string>

class lua_State;

namespace api {

class lua_wrapper {
public:
    typedef std::function<int (lua_State *)> wrapped_function_t;
private:
    lua_State *m_state;
public:
    lua_wrapper();
    ~lua_wrapper();

    void add_api_function(std::string name,
        const wrapped_function_t &function);
private:
    void get_table(std::string name);
    void set_global(std::string name);

    static int call_function(lua_State *state);
    static int gc_function(lua_State *state);
};

} // namespace api

#endif
