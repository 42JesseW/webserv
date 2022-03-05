project(webserv)

cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 98)

include_directories(includes)

file(GLOB_RECURSE SOURCES srcs/*.cpp srcs/**/*.cpp)

add_executable(webserv ${SOURCES})

add_subdirectory(tests)
