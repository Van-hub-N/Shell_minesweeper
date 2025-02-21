cmake_minimum_required(VERSION 3.16)
project(Minesweeper)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_SOURCE_DIR minesweeper)
set(CMAKE_BINARY_DIR build)
set(CMAKE_INSTALL_PREFIX devel)





enable_testing()
add_subdirectory(minesweeper)


