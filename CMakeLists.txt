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
    option(ENABLE_ANSI_CONSTRUCTOR "Enables windows ansi support with a constructor" FALSE) # currently not working
    target_compile_definitions(c_terminal PRIVATE USE_ANSI)
    if(ENABLE_ANSI_SUPPORT)
        target_compile_definitions(c_terminal PRIVATE USE_ANSI_CONSTRUCTOR)
    endif()
endif()

target_include_directories(c_terminal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
