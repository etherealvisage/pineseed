cmake_minimum_required(VERSION 2.6)

project(pineseed C CXX)

include_directories(libs/lua-include)
link_directories(${CMAKE_CURRENT_LIST_DIR}/libs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(src)
