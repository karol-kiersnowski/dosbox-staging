libints_sources = files('''
    int10.cpp
    int10_memory.cpp
    int10_misc.cpp
    int10_put_pixel.cpp
    int10_video_state.cpp
    int10_vptable.cpp
    bios_disk.cpp
    bios_keyboard.cpp
    ems.cpp
    int10_char.cpp
    int10_pal.cpp
    int10_vesa.cpp
    bios.cpp
    int10_modes.cpp
    xms.cpp
    mouse.cpp
    '''.split())

libints = static_library('ints', libints_sources,
                         include_directories : incdir,
                         dependencies : [sdl2_dep])
