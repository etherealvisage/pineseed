add_definitions(-Wextra -Wall -std=c++1y)

aux_source_directory(. dot_sources)
aux_source_directory(game game_sources)

include_directories(.)

set(sources
    ${dot_sources}
)

add_executable(pineseed ${sources} ${game_sources})

target_link_libraries(pineseed m kriti ${kritiLibraries})
