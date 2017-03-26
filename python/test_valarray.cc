#include <valarray>

#include <iostream>
#include <fstream>


void test_function( std::valarray<int> & a )
{
  std::cout << "Hallo" << std::endl;

  for (int i=0;i<a.size();++i)
    std::cout << a[i] << std::endl;

  if ( a.size() > 0 )
    a[0] = -1;
}
