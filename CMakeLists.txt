cmake_minimum_required(VERSION 3.0.0)
project(
    c_terminal
    VERSION 1.0.0
    HOMEPAGE_URL "https://github.com/NilEis/C-Terminal"
    LANGUAGES C
    )

set(SOURCE_FILES_LIST src/terminal.c)
option(BUILD_SHARED_LIBS "Build a shared libary" FALSE)

add_library(c_terminal ${SOURCE_FILES_LIST})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
