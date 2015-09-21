cmake_minimum_required(VERSION 2.6)

project(pineseed C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

include_directories(kriti)
link_directories(${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(src)
