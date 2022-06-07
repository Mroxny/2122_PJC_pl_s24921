cmake_minimum_required(VERSION 3.21)
project(2122_PJC_pl_s24921)

set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_STANDARD 20)

add_executable(2122_PJC_pl_s24921 main.cpp creatures.hpp creatures.cpp engine.cpp engine.hpp game.hpp game.cpp)
