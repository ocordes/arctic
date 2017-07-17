//#define BOOST_TEST_MODULE strstr

#include <boost/test/unit_test.hpp>

#include "image_fits.hh"

// written by: Oliver Cordes 2017-05-31
// changed by: Oliver Cordes 2017-07-17


BOOST_AUTO_TEST_SUITE( image_fits_test_suite )

// good constructor
BOOST_AUTO_TEST_CASE( constructor )
{
  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );
  argv_test[1] = strdup( "infile" );
  argv_test[2] = strdup( "outfile" );

  BOOST_CHECK_THROW( fits_image im1( 2, argv_test ), char const* );
  fits_image im( 3, argv_test );

  free( argv_test[2] );
  free( argv_test[1] );
  free( argv_test[0] );
  free( argv_test );

  BOOST_CHECK_EQUAL( im.prgname, "Program_name_test infile outfile" );
}


// check the existence of the clock_charge_prepare
BOOST_AUTO_TEST_CASE( clock_charge_prepare_test )
{
  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );
  argv_test[1] = strdup( "infile" );
  argv_test[2] = strdup( "outfile" );

  fits_image im( 3, argv_test );

  free( argv_test[2] );
  free( argv_test[1] );
  free( argv_test[0] );
  free( argv_test );

  BOOST_CHECK_EQUAL( im.clock_charge_prepare(), 0 );
}


BOOST_AUTO_TEST_SUITE_END()
