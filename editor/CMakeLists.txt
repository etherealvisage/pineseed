add_definitions(-Wall -Wextra -std=c++11 -g)

find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

aux_source_directory(. dotSources)

set(sources
    ${dotSources}
)

add_executable(pineseed_editor ${sources})
target_link_libraries(pineseed_editor Qt5::Widgets)
