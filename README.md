proteus
=======

How to use:
===========
  `proteus robot.cfg python.tmpl` will create the python example api.
  `proteus -h` will list the availble commands. You can provide more than one configuration and template file at the same time, and they will all be processed. Sample template and configuration files can be found in the project root directory and in the tool/tests subdirectory.

How to build:
============
  After you have installed all of the dependencies. Use `scons proteus` to build the proteus tool. `scons lib` to build libproteus, which contais the ASR code.
  
Dependencies:
============
- scons
- clang 3.3 or higher
- boost 1.54 or higher
- port-audio 2.0
- pocketsphinx
- sphinxbase
- libblas
- lapack
- libsamplerate
- libsndfile
