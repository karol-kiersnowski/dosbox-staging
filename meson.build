project('dosbox-staging', 'c', 'cpp',
        default_options : ['c_std=c99', 'cpp_std=c++14'])

# config file
conf_data = configuration_data()
conf_data.set('version', '0.76.0')
configure_file(input : 'src/config.h.in',
               output : 'config.h',
               configuration : conf_data)

# subdirs
subdir('src/libs/nuked')

# external dependencies
sdl2_dep = dependency('sdl2')

# internal dependencies
libnuked_dep = declare_dependency(link_with : libnuked)

# binary
executable('dosbox', 'src/main.cpp', dependencies : [ sdl2_dep, libnuked_dep ])
