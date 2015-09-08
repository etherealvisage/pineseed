add_definitions(-Wextra -Wall -std=c++11)

aux_source_directory(. dot_sources)
aux_source_directory(api api_sources)
aux_source_directory(interface interface_sources)
aux_source_directory(lua_api lua_api_sources)

include_directories(.)

set(sources
    ${dot_sources}
    ${api_sources}
    ${interface_sources}
    ${lua_api_sources}
)

add_executable(pineseed ${sources})

target_link_libraries(pineseed m SDL SDL_gfx)
