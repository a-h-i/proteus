import os
import glob
def make_package(env, tag, package) :
    objs = SConscript(os.path.join(package,"SConscript"), exports={'env' : env, 'tag' :  tag})
    return objs







# Add libs here
libs = Split( ' -lboost_regex -lboost_filesystem -lboost_system' )
env = Environment(CXX = 'clang++', ENV = {'PATH' : os.environ['PATH']}, LINKFLAGS='-fPIC', LIBS=libs)
env['ENV']['TERM'] = os.environ['TERM']
release = env.Clone()
release['CXXFLAGS'] = '-Wall -Werror -std=c++11 -O3 -fPIE'

debug = env.Clone()
debug['CXXFLAGS'] = '-Wall -Werror -std=c++11 -g -fPIE -DDEBUG_ME_SOFTLY'



packages = ['src/generator', 'src/ui', 'src/tests']

# Objects
d_objs = [make_package(debug, 'dbg', p) for p in packages]
r_objs = [make_package(release, 'rel', p) for p in packages]




# Targets
d_target = debug.Program('proteus-dbg', d_objs)
r_target = release.Program('proteus', r_objs)





#library
linkflags = ' `pkg-config --libs pocketsphinx` ' + ' `pkg-config --libs portaudio-2.0` -fpic'

libenv = Environment(CXX = 'clang++', ENV = {'PATH' : os.environ['PATH']}, LINKFLAGS=linkflags)
libenv['ENV']['TERM'] = os.environ['TERM']
libenv['CXXFLAGS'] = '-Wall -Werror -fpic -std=c++11 -O3   -I/usr/include/sphinxbase -I/usr/include/pocketsphinx -I/usr/include/x86_64-linux-gnu -I/usr/include/x86_64-linux-gnu/sphinxbase '
lib_packages = ['src/recognizer', 'src/recognizer/recorder']
lib_objs = [make_package(libenv, 'lib', p) for p in lib_packages]
l_target = libenv.SharedLibrary("proteus", lib_objs)
libenv.Alias('lib', [l_target])