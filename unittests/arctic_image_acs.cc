//#define BOOST_TEST_MODULE strstr

#include <boost/test/unit_test.hpp>

#include <CCfits/CCfits>

#include "image_acs.hh"
#include "output.hh"

using namespace CCfits;

// written by: Oliver Cordes 2017-05-31
// changed by: Oliver Cordes 2017-07-18

bool check_array( std::valarray<double> & a, std::valarray<double> & b, double limits )
{
  for (unsigned int i=0; i<a.size();++i)
  {
    if ( fabs( a[i] - b[i] ) > limits )
    {
      std::cout << std::fixed << std::setprecision(9)
                << "Error in array [" << i << "]: a=" << a[i] <<  " b=" << b[i]
                << " diff=" << fabs( a[i] - b[i] ) << " limits=" << limits << std::endl;
      return false;
    }
  }

  return true;
}


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

// create a fits file and use the normal arctic flow to return
// the CTE algorithm (without saving)
// - NEO algorithm
BOOST_AUTO_TEST_CASE( clock_charge_test1 )
{
  std::string in_filename = "image_clock_charge_test1.fits";

  long naxes[2] = { 1, 10 };

  std::valarray<double> in_data =  { 40.787811279, 42.996334076, 36.101192474,
                                      34.036590576, 28.182380676, 33.998012543,
                                      16.299722672, 32.821144104, 30.215837479,
                                      30.543056488 };

  std::valarray<double> out_data = { 40.837474823, 43.048473358, 36.146598816,
                                      34.080699921, 28.220380783, 34.049839020,
                                      16.316484451, 32.891876221, 30.255695343,
                                      30.585535049 };

  {
    std::unique_ptr<FITS> pFits;

    int naxis = 2;

    std::string filename( "!"+in_filename );
    pFits.reset( new FITS( filename , FLOAT_IMG , naxis, naxes ) );

    PHDU& hdu = pFits->pHDU();

    hdu.addKey( "DATE", 3506.238668981474, "" );
    hdu.addKey( "BUNIT", "ELECTRONS", "" );

    long  fpixel(1);  // ???? what the heck is this useful ...
    hdu.write( fpixel, naxes[0]*naxes[1], in_data );
  }
  //  FITS file is now written

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );
  argv_test[1] = strdup( in_filename.c_str() );
  argv_test[2] = strdup( "outputfile" );

  acs_image im( 3, argv_test );
  im.parameters->algorithm = ALGORITHM_NEO;

  free( argv_test[2] );
  free( argv_test[1] );
  free( argv_test[0] );
  free( argv_test );

  debug_level = 10;
  BOOST_CHECK_EQUAL( im.read_file(), 0 );
  BOOST_CHECK_EQUAL( im.clock_charge(), 0 );
  debug_level = 0;

  BOOST_CHECK_EQUAL( check_array( im.image_data, out_data, 1e-5), true );

  unlink( in_filename.c_str() );
}

// create a fits file and use the normal arctic flow to return
// the CTE algorithm (without saving)
// - CLASSIC algorithm
BOOST_AUTO_TEST_CASE( clock_charge_test2 )
{
  std::string in_filename = "image_clock_charge_test2.fits";

  long naxes[2] = { 1, 10 };

  std::valarray<double> in_data =  { 40.787811279, 42.996334076, 36.101192474,
                                      34.036590576, 28.182380676, 33.998012543,
                                      16.299722672, 32.821144104, 30.215837479,
                                      30.543056488 };

  std::valarray<double> out_data = { 40.837474823, 43.048473358, 36.146598816,
                                      34.080699921, 28.220380783, 34.049839020,
                                      16.316484451, 32.891876221, 30.255695343,
                                      30.585535049 };

  {
    std::unique_ptr<FITS> pFits;

    int naxis = 2;

    std::string filename( "!"+in_filename );
    pFits.reset( new FITS( filename , FLOAT_IMG , naxis, naxes ) );

    PHDU& hdu = pFits->pHDU();

    hdu.addKey( "DATE", 3506.238668981474, "" );
    hdu.addKey( "BUNIT", "ELECTRONS", "" );

    long  fpixel(1);  // ???? what the heck is this useful ...
    hdu.write( fpixel, naxes[0]*naxes[1], in_data );
  }
  //  FITS file is now written

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );
  argv_test[1] = strdup( in_filename.c_str() );
  argv_test[2] = strdup( "outputfile" );

  acs_image im( 3, argv_test );
  im.parameters->algorithm = ALGORITHM_CLASSIC;

  free( argv_test[2] );
  free( argv_test[1] );
  free( argv_test[0] );
  free( argv_test );

  debug_level = 10;
  BOOST_CHECK_EQUAL( im.read_file(), 0 );
  BOOST_CHECK_EQUAL( im.clock_charge(), 0 );
  debug_level = 0;

  BOOST_CHECK_EQUAL( check_array( im.image_data, out_data, 1e-5), true );

  unlink( in_filename.c_str() );
}

// create a fits file and use the normal arctic flow to return
// the CTE algorithm (without saving)
// - NEO2 algorithm
BOOST_AUTO_TEST_CASE( clock_charge_test3 )
{
  std::string in_filename = "image_clock_charge_test3.fits";

  long naxes[2] = { 1, 10 };

  std::valarray<double> in_data =  { 40.787811279, 42.996334076, 36.101192474,
                                      34.036590576, 28.182380676, 33.998012543,
                                      16.299722672, 32.821144104, 30.215837479,
                                      30.543056488 };

  std::valarray<double> out_data = { 40.837473022, 43.048471768, 36.146614489,
                                      34.080822266, 28.220484291, 34.049768026,
                                      16.316589522, 32.891894026, 30.255838231,
                                      30.585432681 };

  {
    std::unique_ptr<FITS> pFits;

    int naxis = 2;

    std::string filename( "!"+in_filename );
    pFits.reset( new FITS( filename , FLOAT_IMG , naxis, naxes ) );

    PHDU& hdu = pFits->pHDU();

    hdu.addKey( "DATE", 3506.238668981474, "" );
    hdu.addKey( "BUNIT", "ELECTRONS", "" );

    long  fpixel(1);  // ???? what the heck is this useful ...
    hdu.write( fpixel, naxes[0]*naxes[1], in_data );
  }
  //  FITS file is now written

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );
  argv_test[1] = strdup( in_filename.c_str() );
  argv_test[2] = strdup( "outputfile" );

  acs_image im( 3, argv_test );
  im.parameters->algorithm = ALGORITHM_NEO2;

  free( argv_test[2] );
  free( argv_test[1] );
  free( argv_test[0] );
  free( argv_test );

  debug_level = 10;
  BOOST_CHECK_EQUAL( im.read_file(), 0 );
  BOOST_CHECK_EQUAL( im.clock_charge(), 0 );
  debug_level = 0;

  BOOST_CHECK_EQUAL( check_array( im.image_data, out_data, 1e-5), true );

  unlink( in_filename.c_str() );
}

// create a fits file and use the normal arctic flow to return
// the CTE algorithm (without saving)
// - default algorithm
BOOST_AUTO_TEST_CASE( clock_charge_test4 )
{
  std::string in_filename = "image_clock_charge_test4.fits";

  long naxes[2] = { 1, 10 };

  std::valarray<double> in_data =  { 40.787811279, 42.996334076, 36.101192474,
                                      34.036590576, 28.182380676, 33.998012543,
                                      16.299722672, 32.821144104, 30.215837479,
                                      30.543056488 };

  std::valarray<double> out_data = { 40.837474823, 43.048473358, 36.146598816,
                                      34.080699921, 28.220380783, 34.049839020,
                                      16.316484451, 32.891876221, 30.255695343,
                                      30.585535049 };

  {
    std::unique_ptr<FITS> pFits;

    int naxis = 2;

    std::string filename( "!"+in_filename );
    pFits.reset( new FITS( filename , FLOAT_IMG , naxis, naxes ) );

    PHDU& hdu = pFits->pHDU();

    hdu.addKey( "DATE", 3506.238668981474, "" );
    hdu.addKey( "BUNIT", "ELECTRONS", "" );

    long  fpixel(1);  // ???? what the heck is this useful ...
    hdu.write( fpixel, naxes[0]*naxes[1], in_data );
  }
  //  FITS file is now written

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );
  argv_test[1] = strdup( in_filename.c_str() );
  argv_test[2] = strdup( "outputfile" );

  acs_image im( 3, argv_test );
  im.parameters->algorithm = 100;   // a not existing algorithm

  free( argv_test[2] );
  free( argv_test[1] );
  free( argv_test[0] );
  free( argv_test );

  debug_level = 10;
  BOOST_CHECK_EQUAL( im.read_file(), 0 );
  BOOST_CHECK_EQUAL( im.clock_charge(), 0 );
  debug_level = 0;

  BOOST_CHECK_EQUAL( check_array( im.image_data, out_data, 1e-5), true );

  unlink( in_filename.c_str() );
}

// create a fits file and use the normal arctic flow to return
// the CTE algorithm (without saving)
// - default algorithm
// - failing because of the wrong FITS header
BOOST_AUTO_TEST_CASE( clock_charge_test5 )
{
  std::string in_filename = "image_clock_charge_test5.fits";

  long naxes[2] = { 1, 10 };

  std::valarray<double> in_data =  { 40.787811279, 42.996334076, 36.101192474,
                                      34.036590576, 28.182380676, 33.998012543,
                                      16.299722672, 32.821144104, 30.215837479,
                                      30.543056488 };

  std::valarray<double> out_data = { 40.837474823, 43.048473358, 36.146598816,
                                      34.080699921, 28.220380783, 34.049839020,
                                      16.316484451, 32.891876221, 30.255695343,
                                      30.585535049 };

  {
    std::unique_ptr<FITS> pFits;

    int naxis = 2;

    std::string filename( "!"+in_filename );
    pFits.reset( new FITS( filename , FLOAT_IMG , naxis, naxes ) );

    PHDU& hdu = pFits->pHDU();

    hdu.addKey( "DATE", 3506.238668981474, "" );
    hdu.addKey( "BUNIT", "COUNTS", "" );

    long  fpixel(1);  // ???? what the heck is this useful ...
    hdu.write( fpixel, naxes[0]*naxes[1], in_data );
  }
  //  FITS file is now written

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );
  argv_test[1] = strdup( in_filename.c_str() );
  argv_test[2] = strdup( "outputfile" );

  acs_image im( 3, argv_test );
  im.parameters->algorithm = 100;   // a not existing algorithm

  free( argv_test[2] );
  free( argv_test[1] );
  free( argv_test[0] );
  free( argv_test );

  debug_level = 10;
  BOOST_CHECK_EQUAL( im.read_file(), 0 );
  BOOST_CHECK_EQUAL( im.clock_charge(), 1 );
  debug_level = 0;


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
