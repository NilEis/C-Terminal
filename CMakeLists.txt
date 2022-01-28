cmake_minimum_required(VERSION 3.0.0)
project(c_terminal VERSION 1.0.0)

add_library(c_terminal STATIC src/terminal.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
