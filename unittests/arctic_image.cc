//#define BOOST_TEST_MODULE strstr

#include <boost/test/unit_test.hpp>

#include "image.hh"

// written by: Oliver Cordes 2017-05-31
// changed by: Oliver Cordes 2017-05-31


BOOST_AUTO_TEST_SUITE( image_test_suite )

BOOST_AUTO_TEST_CASE( constructor )
{
  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );

  image im( 1, argv_test );

  free( argv_test[0] );
  free( argv_test );

  BOOST_CHECK_EQUAL( im.prgname, "Program_name_test" );
}


BOOST_AUTO_TEST_SUITE_END()
