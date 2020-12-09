libdos_sources = files('''
    cdrom.cpp
    dos_execute.cpp
    dos_ioctl.cpp
    dos_memory.cpp
    dos_tables.cpp
    cdrom_image.cpp
    dos.cpp
    dos_classes.cpp
    dos_devices.cpp
    dos_files.cpp
    dos_keyboard_layout.cpp
    dos_mscdex.cpp
    drive_cache.cpp
    drive_fat.cpp
    drive_iso.cpp
    drive_virtual.cpp
    drives.cpp
    program_autotype.cpp
    program_ls.cpp
    dos_programs.cpp
    drive_local.cpp
    drive_overlay.cpp
    dos_misc.cpp
    '''.split())

libdos = static_library('dos', libdos_sources,
                        include_directories : incdir,
                        dependencies : [sdl2_dep, threads_dep])
