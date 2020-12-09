libdecoders_sources = files('''
    SDL_sound.c
    flac.c
    mp3.cpp
    mp3_seek_table.cpp
    vorbis.c
    wav.c
    '''.split())
#opus.cpp

libdecoders = static_library('decoders', libdecoders_sources,
                             include_directories : incdir,
                             dependencies : [sdl2_dep])
