cmake_minimum_required(VERSION 3.25)

project(ricochet)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(../submodules/box2d box2d)
add_subdirectory(../submodules/raylib raylib)

set(SOURCES
    main.cpp
    engine.cpp
    systems.cpp
    entities.cpp
    components.cpp
    common.cpp
    assets.cpp)

add_executable(ricochet ${SOURCES})

include_directories(
  PUBLIC
  # submodules
  ../submodules/neat-headers/include
  ../submodules/box2d/include
  ../submodules/raylib/src)

target_link_libraries(ricochet box2d)
target_link_libraries(ricochet raylib)
target_compile_options(ricochet PUBLIC -Wall -Wextra -g -Wno-unknown-pragmas)
