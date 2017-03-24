#!/bin/bash

swig -c++ -python arctic.i 
g++ -c arctic_wrap.cxx -I/Users/ocordes/anaconda/include/python2.7 -I../src
g++ --shared -o _arctic.so arctic_wrap.o ../build/libartic.a  -L/Users/ocordes/anaconda/lib/ -lpython2.7 
