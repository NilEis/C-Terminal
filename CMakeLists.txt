cmake_minimum_required(VERSION 3.0.0)
project(terminal_c VERSION 1.0.0)

add_library(terminal_c STATIC src/terminal.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
