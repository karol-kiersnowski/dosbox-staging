libgui_sources = files('''
    render_scalers.cpp
    sdl_gui.cpp
    sdl_mapper.cpp
    render.cpp
    sdlmain.cpp
'''.split())

# bundled deps
libppscale_dep = declare_dependency(link_with : libppscale)

libgui = static_library('gui', libgui_sources,
                        include_directories : incdir,
                        dependencies : [sdl2_dep, threads_dep, libppscale_dep])
