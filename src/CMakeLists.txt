add_definitions(-Wextra -Wall -std=c++1y)

aux_source_directory(. dot_sources)
aux_source_directory(api api_sources)
aux_source_directory(event event_sources)
aux_source_directory(interface interface_sources)

include_directories(.)

set(sources
    ${dot_sources}
    ${api_sources}
    ${event_sources}
    ${interface_sources}
)

add_executable(pineseed ${sources})

target_link_libraries(pineseed m SDL SDL_gfx SDL_image)
