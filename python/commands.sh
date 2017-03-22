  683  2017-03-22 16:00:32 swig -c++ -python arctic.i 
  684  2017-03-22 16:00:34 g++ -c arctic_wrap.cxx -I/Users/ocordes/anaconda/include/python2.7 -I../src
  693  2017-03-22 16:07:51 g++ --shared -o module.so arctic_wrap.o ../build/libartic.a  -L/Users/ocordes/anaconda/lib/ -lpython2.7 
