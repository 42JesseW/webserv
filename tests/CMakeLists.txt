cmake_minimum_required(VERSION 3.10)
project(tests C CXX)

set(HEADERS catch.hpp)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/includes .)

file(GLOB_RECURSE TEST_SOURCES ../srcs/*.cpp ../srcs/**/*.cpp)

add_library(tester STATIC ${TEST_SOURCES})

file(GLOB_RECURSE SOURCES main.cpp unittests/test_*.cpp)

add_executable(tests ${SOURCES})

target_link_libraries(tests tester)