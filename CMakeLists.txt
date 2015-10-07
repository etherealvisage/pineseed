cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/kriti/cmake/)

project(pineseed C CXX)

add_definitions(-DKRITI_DATA_PATH="data/")
add_definitions(-DKRITI_CONFIG_PATH="kriti.xml")

include(kriti/cmake/kriti-env.cmake)

include_directories(kriti/headers)
link_directories(${CMAKE_CURRENT_LIST_DIR}/kriti)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
add_subdirectory(src)
