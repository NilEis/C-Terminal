cmake_minimum_required(VERSION 3.0.0)
project(
    c_terminal
    VERSION 1.0.0
    HOMEPAGE_URL "https://github.com/NilEis/C-Terminal"
    LANGUAGES C
    )

set(SOURCE_FILES_LIST src/terminal.c)

if (NOT TARGET c_terminal)
    add_library(c_terminal ${SOURCE_FILES_LIST})
endif()

option(ENABLE_ANSI_SUPPORT "Enables windows ansi support" TRUE)

if(ENABLE_ANSI_SUPPORT)
    message(STATUS "Use ANSI coloring")
    option(ENABLE_ANSI_CONSTRUCTOR "Enables windows ansi support with a constructor" TRUE)
    target_compile_definitions(c_terminal PUBLIC C_TERMINAL_USE_ANSI=1)
    if(ENABLE_ANSI_CONSTRUCTOR)
        target_compile_definitions(c_terminal PUBLIC C_TERMINAL_USE_ANSI_CONSTRUCTOR=1 C_TERMINAL_USE_ANSI=1)
    endif()
endif()

target_include_directories(c_terminal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
