cmake_minimum_required(VERSION 2.8.4)
project(httpserver)

set(SOURCE_FILES main.cpp)
find_library(ws2_32_LIBRARY_PATH ws2_32)
add_executable(httpserver ${SOURCE_FILES})
target_link_libraries(httpserver ${ws2_32_LIBRARY_PATH})