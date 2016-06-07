// test_slice.cc
//
// is a test program for the slicing algorithm
//
// written by: Oliver Cordes 2016-05-09
// changed by: Oliver Cordes 2016-05-11
//
// compile: g++ -Wall test_slice.cc image_slice.cc
//

#include <iostream>
#include <cstdlib>

#include "image_slice.hh"

int main( int argc, char* argv[] )
{
  std::cout << "Hello world!" << std::endl;


  std::image_slice is( 100, 20, 5, image_readout_y, image_reverse );

  try {
    is.reset( 9  );
  }

  catch ( std::slice_exception & e )
    {
      std::cout << "BUG in the code!" << std::endl;
      std::cout << e.what() << std::endl;
    }


  
  std::cout << (*is) << std::endl;
  ++is;
  
  std::cout << ++is << std::endl;
  std::cout << is++ << std::endl;
  std::cout << is++ << std::endl;

  std::image_slice i;

  int j = 0;
  for (i=is.begin();i!=is.end();i++)
    {
      std::cout << "i=" << (*i) << std::endl;
      j++;
      //if ( j == 30 )
      //break;
      
    }
  // i++; // this causes an error!!
  return 0;
}

