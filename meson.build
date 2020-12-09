project('dosbox-staging', 'cpp',
        default_options : ['cpp_std=c++14'])

conf_data = configuration_data()
conf_data.set('version', '0.76.0')

executable('dosbox', 'src/main.cpp')

configure_file(input: 'src/config.h.in',
               output: 'config.h',
               configuration: conf_data)
