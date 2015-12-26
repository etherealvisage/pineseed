add_definitions(-Wextra -Wall -std=c++11 -g)

aux_source_directory(. dotSources)
aux_source_directory(conv convSources)
aux_source_directory(game gameSources)
aux_source_directory(level levelSources)
aux_source_directory(scene sceneSources)

include_directories(.)

set(sources
    ${dotSources}
    ${convSources}
    ${gameSources}
    ${levelSources}
    ${sceneSources}
)

add_executable(pineseed ${sources})

target_link_libraries(pineseed m kriti ${kritiLibraries})
