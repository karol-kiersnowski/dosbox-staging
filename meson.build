project('dosbox-staging', 'c', 'cpp',
        default_options : ['c_std=c99', 'cpp_std=c++14'])

# config file
conf_data = configuration_data()
conf_data.set('version', '0.76.0')
configure_file(input : 'src/config.h.in',
               output : 'config.h',
               configuration : conf_data)

# external dependencies
sdl2_dep    = dependency('sdl2')
threads_dep = dependency('threads')

# subdirs
incdir = include_directories('include', '.')
subdir('src/gui')
subdir('src/hardware')
subdir('src/libs/nuked')
subdir('src/midi') # TODO     midi depends on: hardware,
                   #      hardware depends on: cpu, gui
		   #           gui depends on: cpu, ints
subdir('src/misc') #          misc depends on: gui, dos

# bundled dependencies
libnuked_dep = declare_dependency(link_with : libnuked)

# internal libs
libgui_dep      = declare_dependency(link_with : libgui)
libhardware_dep = declare_dependency(link_with : libhardware)
libmidi_dep     = declare_dependency(link_with : libmidi)
libmisc_dep     = declare_dependency(link_with : libmisc)

# binary
executable('dosbox', 'src/main.cpp',
           dependencies : [
	     sdl2_dep,
	     threads_dep,
	     libgui_dep,
	     libhardware_dep,
	     libnuked_dep,
	     libmidi_dep,
	     libmisc_dep,
	   ],
	   include_directories : incdir)
