cmake_minimum_required(VERSION 3.5)

project(p2p-videochat VERSION 0.1 LANGUAGES CXX)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)

enable_testing()
add_subdirectory(test)
