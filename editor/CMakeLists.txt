add_definitions(-Wall -Wextra -std=c++11 -g)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

aux_source_directory(. dotSources)

set(sources
    ${dotSources}
)

qt5_add_resources(resources ${CMAKE_CURRENT_LIST_DIR}/icons.qrc)

add_executable(pineseed_editor ${sources} ${resources})
target_link_libraries(pineseed_editor Qt5::Core Qt5::Widgets Qt5::Xml)
