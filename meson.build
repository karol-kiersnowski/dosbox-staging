project('dosbox-staging', 'cpp',
        default_options : ['cpp_std=c++14'])

# dependencies
sdl2_dep = dependency('sdl2')

# config file
conf_data = configuration_data()
conf_data.set('version', '0.76.0')
configure_file(input : 'src/config.h.in',
               output : 'config.h',
               configuration : conf_data)

# binary
executable('dosbox', 'src/main.cpp', dependencies : sdl2_dep)
