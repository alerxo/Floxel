cmake_minimum_required(VERSION 3.16)

project(Floxel)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output path
set(ARCHITECTURE 32)
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE 64)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${CMAKE_SYSTEM_NAME}_x${ARCHITECTURE}_${CMAKE_BUILD_TYPE})

# Including headers and setting source files
include_directories(include dependencies/glad/include dependencies/Assets/include)
file(GLOB_RECURSE SOURCES src/*.cpp)

# Adding directories to dependency makefiles
add_subdirectory(dependencies/SDL EXCLUDE_FROM_ALL)
add_subdirectory(dependencies/glad)
add_subdirectory(dependencies/Assets)

# Creating executeable with compile options and linked dependencies
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 glad Assets)