add_definitions(-Wextra -Wall -std=c++1y)

aux_source_directory(. dot_sources)
aux_source_directory(game game_sources)

include_directories(.)

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_image REQUIRED SDL2_image>=2.0.0)

find_package(Boost COMPONENTS filesystem system signals)
find_package(OpenGL)
find_package(GLEW)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_image_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

set(sources
    ${dot_sources}
)

add_executable(pineseed ${sources} ${game_sources})

target_link_libraries(pineseed m ${SDL2_LIBRARIES} ${SDL2_image_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${Boost_LIBRARIES} kriti)
