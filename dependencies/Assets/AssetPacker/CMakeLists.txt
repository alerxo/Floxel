cmake_minimum_required(VERSION 3.16)

project(AssetPacker)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Setting source file
file(GLOB SOURCES src/*.cpp)

# Creating executeable
add_executable(${PROJECT_NAME} ${SOURCES})