cmake_minimum_required(VERSION 3.10)
project(tests CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/includes)

file(GLOB_RECURSE TEST_SOURCES ../srcs/*.cpp ../srcs/**/*.cpp)

# remove main.cpp from the TEST_SOURCES list
list(REMOVE_ITEM TEST_SOURCES ../srcs/main.cpp)

add_library(tester STATIC ${TEST_SOURCES})

file(GLOB_RECURSE SOURCES unittests/main.cpp unittests/test_*.cpp)

add_executable(tests ${SOURCES})

target_link_libraries(tests tester)
