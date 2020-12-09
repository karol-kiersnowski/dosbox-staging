libcpu_sources = files('''
    callback.cpp
    core_normal.cpp
    core_prefetch.cpp
    core_simple.cpp
    flags.cpp
    modrm.cpp
    core_dyn_x86.cpp
    core_full.cpp
    cpu.cpp
    paging.cpp
    core_dynrec.cpp
    '''.split())

libcpu = static_library('cpu', libcpu_sources,
                        include_directories : incdir,
                        dependencies : sdl2_dep)
