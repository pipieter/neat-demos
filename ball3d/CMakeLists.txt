cmake_minimum_required(VERSION 3.25)

project(ball3d)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(../submodules/JoltPhysics/Build JoltPhysics)
add_subdirectory(../submodules/raylib raylib)

set(SOURCES main.cpp systems.cpp entities.cpp)

add_executable(ball3d ${SOURCES})

include_directories(
  PUBLIC
  # submodules
  ../submodules/neat-headers/include
  ../submodules/raylib/src
  ../submodules/JoltPhysics/Jolt)

target_link_libraries(ball3d raylib)
target_link_libraries(ball3d Jolt)
target_compile_options(ball3d PUBLIC -Wall -Wextra -g -Wno-unknown-pragmas)
