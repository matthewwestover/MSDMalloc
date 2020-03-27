cmake_minimum_required(VERSION 3.15)
project(MSDMalloc)

set(CMAKE_CXX_STANDARD 17)

add_executable(testing catch.hpp MSDHashTable.cpp MSDHashTable.hpp MSDMalloc.cpp MSDMalloc.hpp testing.cpp)