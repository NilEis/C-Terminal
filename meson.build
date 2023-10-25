project('c_terminal', 'c')

incdir_terminal = include_directories('include')

if get_option('enableAnsiSupport')
    add_project_arguments('-DC_TERMINAL_USE_ANSI', language : 'c')
endif

c_terminal = library('c_terminal', 'src/terminal.c', include_directories: incdir_terminal)