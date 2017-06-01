//#define BOOST_TEST_MODULE strstr

#include <boost/test/unit_test.hpp>

#include "image_acs.hh"

#include <CCfits/CCfits>

using namespace CCfits;

// written by: Oliver Cordes 2017-05-31
// changed by: Oliver Cordes 2017-05-31


BOOST_AUTO_TEST_SUITE( image_acs_test_suite )

BOOST_AUTO_TEST_CASE( constructor )
{
  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );
  argv_test[1] = strdup( "infile" );
  argv_test[2] = strdup( "outfile" );

  BOOST_CHECK_THROW( acs_image im( 1, argv_test ), char const* );

  acs_image im1( 3, argv_test );

  free( argv_test[2] );
  free( argv_test[1] );
  free( argv_test[0] );
  free( argv_test );

  BOOST_CHECK_EQUAL( im1.prgname, "Program_name_test infile outfile" );
}

BOOST_AUTO_TEST_CASE( clock_charge_prepare_test )
{
  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );
  argv_test[1] = strdup( "infile" );
  argv_test[2] = strdup( "outfile" );

  acs_image im( 3, argv_test );

  free( argv_test[2] );
  free( argv_test[1] );
  free( argv_test[0] );
  free( argv_test );

  //std::unique_ptr<FITS> pFits;

  int naxis = 2;
  long naxes[2] = { 10, 10 };

  std::string filename = "!clock_charge_prepare_test.fits";

  im.FITS_image.reset( new FITS( filename , FLOAT_IMG , naxis, naxes ) );
}

BOOST_AUTO_TEST_CASE( date2double_test )
{
  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );
  argv_test[1] = strdup( "infile" );
  argv_test[2] = strdup( "outfile" );

  acs_image im( 3, argv_test );

  free( argv_test[2] );
  free( argv_test[1] );
  free( argv_test[0] );
  free( argv_test );

  std::string s = "2000-01-01";

  BOOST_CHECK_EQUAL( im.date2double( s ), 2451544.5 );
}

BOOST_AUTO_TEST_SUITE_END()
