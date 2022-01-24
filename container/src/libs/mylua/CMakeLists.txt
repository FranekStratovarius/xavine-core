set(NAME "mylua")
project(${NAME})
cmake_minimum_required(VERSION 3.00)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-Wall")

set(CMAKE_BUILD_TYPE Release)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/include)

file(GLOB_RECURSE SOURCES_C src/c/*.c)

add_library(${NAME} ${SOURCES_C})

