cmake_minimum_required(VERSION 3.25)

project(ricochet)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(submodules/box2d)
add_subdirectory(submodules/raylib)

set(SOURCES src/main.cpp src/engine.cpp src/systems.cpp src/entities.cpp
            src/components.cpp src/common.cpp)

add_executable(ricochet ${SOURCES})

include_directories(
  PUBLIC
  # submodules
  submodules/neat-headers/include
  submodules/box2d/include
  submodules/raylib/src)

target_link_libraries(ricochet box2d)
target_link_libraries(ricochet raylib)
target_compile_options(ricochet PUBLIC -Wall -Wextra -g -Wno-unknown-pragmas)
