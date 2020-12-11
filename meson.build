project('dosbox-staging',
	['c', 'cpp'],
	meson_version : '>= 0.55.0',
	default_options : [
	  'cpp_std=c++14',
	  'glib:default_library=static',
	  'glib:bsymbolic_functions=false',
	  'glib:fam=false',
	  'glib:glib_assert=false',
	  'glib:installed_tests=false',
	  'glib:internal_pcre=true',
	  'glib:libmount=disabled',
	  'glib:nls=disabled',
	  'glib:selinux=disabled',
	  'glib:xattr=false',
	  'fluidsynth:default_library=static'])

# Note:
#   Setting the language standard using the built-in option 'c_std=c99'
#   applies it to all subproject builds without ability to override it
#   at the subproject level.  So we must apply a flag here.
#   https://github.com/mesonbuild/meson/issues/2612
add_project_arguments('-std=c99', language : 'c')

# modules
cmake = import('cmake')

# config file
conf_data = configuration_data()
conf_data.set('version', '0.76.0')
configure_file(input : 'src/config.h.in',
	       output: 'config.h',
	       configuration : conf_data)

# external dependencies
sdl2_dep       = dependency('sdl2')
threads_dep    = dependency('threads')
glib_dep       = dependency('glib-2.0', fallback : ['glib'])
gthread_dep    = dependency('gthread-2.0', fallback : ['glib'])

# FluidSynth subproject
fluidsynth_opts = cmake.subproject_options()
fluidsynth_opts.add_cmake_defines({
	'BUILD_SHARED_LIBS': 0,
	'CMAKE_BUILD_TYPE': 'Release',
	'enable-alsa': 0,
	'enable-aufile': 0,
	'enable-bus': 0,
	'enable-coreaudio': 0,
	'enable-coremidi': 0,
	'enable-dsound': 0,
	'enable-floats': 1,
	'enable-ipv6': 0,
	'enable-jack': 0,
	'enable-libsndfile': 0,
	'enable-midishare': 0,
	'enable-network': 0,
	'enable-oss': 0,
	'enable-portaudio': 0,
	'enable-pulseaudio': 0,
	'enable-readline': 0,
	'enable-sdl2': 0,
	'enable-waveout': 0,
	'enable-winmidi': 0})

fluidsynth_proj = cmake.subproject('fluidsynth', options: fluidsynth_opts)

# fluidsynth_dep = dependency('fluidsynth', fallback : ['fluidsynth'])


#fluidsynth_proj.dependency('glib-2.0', fallback: ['glib'])
#fluidsynth_proj.dependency('gthread-2.0', fallback: ['glib'])
# fluidsynth = dependency('fluidsynth', fallback : ['fluidsynth', 'fluidsynth_dep'])

#fluidsynth_dep = fluidsynth_proj.dependency(
#	'fluidsynth',
#	version : '>=2')
#	fallback : ['fluidsynth'])

# fluidsynth_proj.dependency(
#	'fluidsynth',
#	version : '>=2',
#	fallback : ['fluidsynth'],
#	dependencies : ['glib-2.0', 'gthread-2.0'])

# includes
incdir = include_directories('include', '.')

# bundled dependencies
subdir('src/libs/decoders')
subdir('src/libs/nuked')
subdir('src/libs/ppscale')

# subdirs
subdir('src/cpu')
subdir('src/gui')
subdir('src/hardware')
subdir('src/ints')
subdir('src/dos')
subdir('src/midi') # TODO     midi depends on: hardware,
                   #      hardware depends on: cpu, gui
		   #           gui depends on: cpu, ints
subdir('src/misc') #          misc depends on: gui, dos
subdir('src/shell')

# bundled dependencies
libdecoders_dep = declare_dependency(link_with : libdecoders)
libnuked_dep    = declare_dependency(link_with : libnuked)
libppscale_dep  = declare_dependency(link_with : libppscale)

# internal libs
libcpu_dep      = declare_dependency(link_with : libcpu)
libdos_dep      = declare_dependency(link_with : libdos)
libgui_dep      = declare_dependency(link_with : libgui)
libhardware_dep = declare_dependency(link_with : libhardware)
libints_dep     = declare_dependency(link_with : libints)
libmidi_dep     = declare_dependency(link_with : libmidi)
libmisc_dep     = declare_dependency(link_with : libmisc)
libshell_dep    = declare_dependency(link_with : libshell)

# binary
bin_sources = ['src/main.cpp', 'src/dosbox.cpp']
executable('dosbox',
	   bin_sources,
	   dependencies : [
	     sdl2_dep,
	     threads_dep,
	     glib_dep,
	     gthread_dep,
#	     fluidsynth_dep,
	     libcpu_dep,
	     libgui_dep,
	     libhardware_dep,
	     libnuked_dep,
	     libmidi_dep,
	     libmisc_dep,
	     libshell_dep,
	   ],
	   include_directories : incdir)

