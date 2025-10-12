cmake_minimum_required(VERSION 3.25)

project(ball3d)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(../submodules/JoltPhysics/Build JoltPhysics)
add_subdirectory(../submodules/raylib raylib)
add_subdirectory(../submodules/assimp assimp)
add_subdirectory(../submodules/r3d r3d)

set(SOURCES main.cpp systems.cpp entities.cpp physics.cpp)

add_executable(ball3d ${SOURCES})

include_directories(
  PUBLIC
  # submodules
  ../submodules/neat-headers/include
  ../submodules/raylib/src
  ../submodules/JoltPhysics/Jolt
  ../submodules/r3d/include
  ../submodules/assimp/include)

target_link_libraries(ball3d raylib)
target_link_libraries(ball3d assimp)
target_link_libraries(ball3d r3d)
target_link_libraries(ball3d Jolt)
target_compile_options(ball3d PUBLIC -Wall -Wextra -g -Wno-unknown-pragmas)
