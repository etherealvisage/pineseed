find_package(Boost COMPONENTS filesystem system signals)

find_package(Bullet)
include_directories(${BULLET_INCLUDE_DIRS})

find_package(SDL2)
include_directories(${SDL2_INCLUDE_DIR})

find_package(SDL2_image)
include_directories(${SDL2IMAGE_INCLUDE_DIR})

find_package(OpenGL)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLEW)
include_directories(${GLEW_INCLUDE_DIR})

find_package(Freetype)
include_directories(${FREETYPE_INCLUDE_DIRS})

set(kritiLibraries 
    ${Boost_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${SDL2_LIBRARY}
    ${SDL2IMAGE_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${FREETYPE_LIBRARY}
    # for clock_gettime().
    rt
)
