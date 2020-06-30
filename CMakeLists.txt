
cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_STANDARD 11)

include(CheckCCompilerFlag)

project(cluster)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

include_directories("inc")
file(GLOB SOURCES "src/*.cpp")

add_executable(cluster ${SOURCES})