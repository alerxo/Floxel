cmake_minimum_required(VERSION 3.16)

project(Assets)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Setting paths
set(NINJA ${CMAKE_SOURCE_DIR}/dependencies/Assets/AssetPacker/build/build.ninja)
set(ASSET_PACKER ${CMAKE_SOURCE_DIR}/dependencies/Assets/AssetPacker/build/AssetPacker.exe)

# Get resource files
file(GLOB_RECURSE RESOURCES ${CMAKE_SOURCE_DIR}/resources/*.*)

# Including headers and setting source file
include_directories(${CMAKE_SOURCE_DIR}/dependencies/Assets/include)
set(SOURCE ${CMAKE_SOURCE_DIR}/dependencies/Assets/src/Assets.cpp)

# Creating static library
add_library(${PROJECT_NAME} STATIC ${SOURCE})

# Adding dependency on generating build files
add_custom_command(
  OUTPUT ${NINJA}
  COMMAND cmake --preset base
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies/Assets/AssetPacker
  DEPENDS ${CMAKE_SOURCE_DIR}/dependencies/Assets/AssetPacker/src/AssetPacker.cpp
)

# Adding dependency on building asset packer
add_custom_command(
  OUTPUT ${ASSET_PACKER}
  COMMAND ninja
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies/Assets/AssetPacker/build
  DEPENDS ${NINJA}
)

# Adding dependency on running asset packer
add_custom_command(
  OUTPUT ${SOURCE}
  COMMAND ./dependencies/Assets/AssetPacker/build/AssetPacker.exe
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  DEPENDS ${RESOURCES} ${ASSET_PACKER}
)