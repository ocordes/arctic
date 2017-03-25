#!/bin/bash

set -x

g++ -c -o test_valarray.o test_valarray.cc
swig -c++ -python arctic.i
g++ -c arctic_wrap.cxx -I/Users/ocordes/anaconda/include/python2.7 -I../src -I/Users/ocordes/anaconda/lib/python2.7/site-packages/numpy/core/include/
g++ --shared -o _arctic.so arctic_wrap.o  test_valarray.o ../build/libartic.a  -L/Users/ocordes/anaconda/lib/ -lpython2.7
