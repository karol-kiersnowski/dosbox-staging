libmidi_sources = [
  'midi.cpp',
  'midi_fluidsynth.cpp',
]

# internal libs
libhardware_dep = declare_dependency(link_with : libhardware)

libmidi = static_library('midi', libmidi_sources,
                         include_directories : incdir,
                         dependencies : [sdl2_dep, libhardware_dep])
