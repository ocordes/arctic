//#define BOOST_TEST_MODULE strstr

#include <boost/test/unit_test.hpp>

#include <CCfits/CCfits>

#include "image_acs.hh"

using namespace CCfits;

// written by: Oliver Cordes 2017-05-31
// changed by: Oliver Cordes 2017-06-04


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

  acs_image im( 3, argv_test );

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

  acs_image im( 3, argv_test );

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

  acs_image im( 3, argv_test );

  free( argv_test[2] );
  free( argv_test[1] );
  free( argv_test[0] );
  free( argv_test );

  BOOST_CHECK_EQUAL( im.read_file(), 0 );

  BOOST_CHECK_EQUAL( im.image_width, naxes[0] );
  BOOST_CHECK_EQUAL( im.image_height, naxes[1] );

  BOOST_CHECK_EQUAL( im.clock_charge_prepare(), 1 );

  unlink( in_filename.c_str() );
}


BOOST_AUTO_TEST_CASE( clock_charge_test )
{
  std::string in_filename = "image_read_test.fits";

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
  argv_test[2] = strdup( "outputfile" );

  acs_image im( 3, argv_test );
  im.parameters = std::shared_ptr<params>( new params() );
  im.parameters->algorithm = ALGORITHM_NEO;

  free( argv_test[2] );
  free( argv_test[1] );
  free( argv_test[0] );
  free( argv_test );

  //im.infilename = in_filename;

  //BOOST_CHECK_EQUAL( im.read_file(), 0 );
  //BOOST_CHECK_EQUAL( im.clock_charge(), 0 );

  unlink( in_filename.c_str() );
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

  s = "2000/01/01";  // malicious format

  BOOST_CHECK_EQUAL( im.date2double( s ), 0.0 );
}

BOOST_AUTO_TEST_SUITE_END()
