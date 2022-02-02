cmake_minimum_required(VERSION 3.17)
project(webserv_github)

set(CMAKE_CXX_STANDARD 98)

include_directories(includes)

file(GLOB_RECURSE SOURCES srcs/*.cpp)

add_executable(webserv ${SOURCES})
