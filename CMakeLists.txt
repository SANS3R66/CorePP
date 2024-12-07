cmake_minimum_required(VERSION 3.10)
project(CorePP)

set(CMAKE_BUILD_TYPE Release)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
endif()

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
include_directories(include/**)