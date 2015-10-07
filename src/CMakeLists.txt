add_definitions(-Wextra -Wall -std=c++11 -g)

aux_source_directory(. dotSources)
aux_source_directory(conv convSources)
aux_source_directory(game gameSources)

include_directories(.)

set(sources
    ${dotSources}
    ${convSources}
    ${gameSources}
)

add_executable(pineseed ${sources})

target_link_libraries(pineseed m kriti ${kritiLibraries})
