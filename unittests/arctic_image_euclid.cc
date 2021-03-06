//#define BOOST_TEST_MODULE strstr

#include <boost/test/unit_test.hpp>

#include <CCfits/CCfits>

#include "image_euclid.hh"

using namespace CCfits;

// written by: Oliver Cordes 2017-05-31
// changed by: Oliver Cordes 2017-05-31


BOOST_AUTO_TEST_SUITE( image_euclid_test_suite )

BOOST_AUTO_TEST_CASE( constructor )
{
  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );
  argv_test[1] = strdup( "infile" );
  argv_test[2] = strdup( "outfile" );

  BOOST_CHECK_THROW( euclid_image im( 2, argv_test ), char const* );

  euclid_image im1( 3, argv_test );

  free( argv_test[2] );
  free( argv_test[1] );
  free( argv_test[0] );
  free( argv_test );

  BOOST_CHECK_EQUAL( im1.prgname, "Program_name_test infile outfile" );
}

BOOST_AUTO_TEST_CASE( clock_charge_prepare_test1 )
{
  std::string in_filename = "clock_charge_prepare_test1.fits";

  long naxes[2] = { 10, 10 };

  {
    std::unique_ptr<FITS> pFits;

    int naxis = 2;

    std::string filename( "!"+in_filename );
    pFits.reset( new FITS( filename , FLOAT_IMG , naxis, naxes ) );

    PHDU& hdu = pFits->pHDU();


    hdu.addKey( "DATE", "2003-01-01", "" );
    hdu.addKey( "BUNIT", "ELECTRONS", "" );
  }
  //  FITS file is now written

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );
  argv_test[1] = strdup( in_filename.c_str() );
  argv_test[2] = strdup( "outfile" );

  euclid_image im( 3, argv_test );

  free( argv_test[2] );
  free( argv_test[1] );
  free( argv_test[0] );
  free( argv_test );

  BOOST_CHECK_EQUAL( im.read_file(), 0 );

  BOOST_CHECK_EQUAL( im.image_width, naxes[0] );
  BOOST_CHECK_EQUAL( im.image_height, naxes[1] );

  BOOST_CHECK_EQUAL( im.clock_charge_prepare(), 0 );

  unlink( in_filename.c_str() );
}

BOOST_AUTO_TEST_CASE( clock_charge_prepare_test2 )
{
  std::string in_filename = "clock_charge_prepare_test2.fits";

  long naxes[2] = { 10, 10 };

  {
    std::unique_ptr<FITS> pFits;

    int naxis = 2;

    std::string filename( "!"+in_filename );
    pFits.reset( new FITS( filename , FLOAT_IMG , naxis, naxes ) );

    PHDU& hdu = pFits->pHDU();

    double date = 3506.238668981474;

    hdu.addKey( "DATE", date, "" );
    hdu.addKey( "BUNIT", "ELECTRONS", "" );
  }
  //  FITS file is now written

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );
  argv_test[1] = strdup( in_filename.c_str() );
  argv_test[2] = strdup( "outfile" );

  euclid_image im( 3, argv_test );

  free( argv_test[2] );
  free( argv_test[1] );
  free( argv_test[0] );
  free( argv_test );

  BOOST_CHECK_EQUAL( im.read_file(), 0 );

  BOOST_CHECK_EQUAL( im.image_width, naxes[0] );
  BOOST_CHECK_EQUAL( im.image_height, naxes[1] );

  BOOST_CHECK_EQUAL( im.clock_charge_prepare(), 0 );

  unlink( in_filename.c_str() );
}

BOOST_AUTO_TEST_CASE( clock_charge_prepare_test3 )
{
  std::string in_filename = "clock_charge_prepare_test3.fits";

  long naxes[2] = { 10, 10 };

  {
    std::unique_ptr<FITS> pFits;

    int naxis = 2;

    std::string filename( "!"+in_filename );
    pFits.reset( new FITS( filename , FLOAT_IMG , naxis, naxes ) );

    PHDU& hdu = pFits->pHDU();


    hdu.addKey( "DATE", "2003-01-01", "" );
    hdu.addKey( "BUNIT", "COUNTS", "" );
  }
  //  FITS file is now written

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );
  argv_test[1] = strdup( in_filename.c_str() );
  argv_test[2] = strdup( "outfile" );

  euclid_image im( 3, argv_test );

  free( argv_test[2] );
  free( argv_test[1] );
  free( argv_test[0] );
  free( argv_test );

  BOOST_CHECK_EQUAL( im.read_file(), 0 );

  BOOST_CHECK_EQUAL( im.image_width, naxes[0] );
  BOOST_CHECK_EQUAL( im.image_height, naxes[1] );

  BOOST_CHECK_EQUAL( im.clock_charge_prepare(), 0 ); // even all is wrong, it is OK, so far

  unlink( in_filename.c_str() );
}

BOOST_AUTO_TEST_SUITE_END()
