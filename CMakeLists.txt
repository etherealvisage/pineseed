cmake_minimum_required(VERSION 2.6)

cmake_policy(SET CMP0012 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

project(pineseed C CXX)

include(build.config)

if(${buildEditor})
    add_subdirectory(editor)
endif(${buildEditor})

if(${buildGame})
    add_definitions(-DKRITI_DATA_PATH="data/")
    add_definitions(-DKRITI_CONFIG_PATH="kriti.xml")

    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/kriti/cmake/)

    include(kriti/cmake/kriti-env.cmake)
    if(NOT ${useDistKriti})
        add_subdirectory(kriti/)
    endif(NOT ${useDistKriti})
    include_directories(kriti/headers)
    link_directories(${CMAKE_CURRENT_LIST_DIR}/kriti)

    add_subdirectory(src)
endif(${buildGame})
