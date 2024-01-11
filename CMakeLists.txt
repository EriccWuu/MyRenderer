# MyRenderer
# |-src
#     |-Geometry.cpp
#     |-Geometry.h
#     |-Model.cpp
#     |-Model.h
#     |-TGAImage.cpp
#     |-TGAImage.h
#     |-Triangle.cpp
#     |-Triangle.h


# cmake_minimum_required(VERSION 3.5)

# # project name
# project(MyRenderer)

# # set source list
# aux_source_directory(./src all_src)

# # add executavle file
# add_executable(${CMAKE_PROJECT_NAME} ${all_src})


cmake_minimum_required (VERSION 3.5)
project(MyRenderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES ./src/*.h ./src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})


