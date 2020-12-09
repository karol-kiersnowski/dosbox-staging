libshell_sources = files('''
    shell.cpp
    shell_batch.cpp
    shell_cmds.cpp
    shell_misc.cpp
    '''.split())

# internal libs
libdos_dep = declare_dependency(link_with : libdos)

libshell = static_library('shell', libshell_sources,
                          include_directories : incdir,
                          dependencies : [sdl2_dep, libdos_dep])
