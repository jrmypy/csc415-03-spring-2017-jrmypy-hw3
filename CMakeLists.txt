cmake_minimum_required(VERSION 2.8.7)
project(hw3)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(-std=c99)

set(SOURCE_FILES main.cpp)
add_executable(hw3 ${SOURCE_FILES})
