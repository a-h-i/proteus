import os
import glob
def make_package(env, tag, package) :
    objs = SConscript(os.path.join(package,"SConscript"), exports={'env' : env, 'tag' :  tag})
    return objs







# Add libs here
libs = Split( ' -lboost_regex -lboost_filesystem -lboost_system' )
env = Environment(CXX = 'clang++', ENV = {'PATH' : os.environ['PATH']}, LINKFLAGS='-fPIE', LIBS=libs)
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
Default(d_target)
