#!/bin/bash
./proteus robot.cfg -Wall cpp.tmpl
clang++ -std=c++11 -Wall -g lib.cpp main.cpp -o robot++ -L. -lproteus -Wl,-rpath,. -lboost_filesystem -lboost_system