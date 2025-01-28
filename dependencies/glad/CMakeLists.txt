cmake_minimum_required(VERSION 3.16)

project(glad)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Including headers and setting source file
include_directories(include)
set(SOURCE src/glad.c)

# Creating static library
add_library(${PROJECT_NAME} STATIC ${SOURCE})