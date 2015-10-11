add_definitions(-Wall -Wextra -std=c++11)

find_package(Qt4 REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

aux_source_directory(. dotSources)

set(sources
    ${dotSources}
)

add_executable(pineseed_editor ${sources})
target_link_libraries(pineseed_editor Qt4::QtGui)
