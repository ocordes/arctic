//#define BOOST_TEST_MODULE strstr

#include <boost/test/unit_test.hpp>

#include "image.hh"
#include "params.hh"

#include <CCfits/CCfits>

#include <unistd.h>
#include <math.h>

using namespace CCfits;

// written by: Oliver Cordes 2017-05-31
// changed by: Oliver Cordes 2017-06-22


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


BOOST_AUTO_TEST_CASE( read_test )
{
  std::string in_filename = "image_read_test.fits";

  long naxes[2] = { 10, 10 };

  {
    std::unique_ptr<FITS> pFits;

    int naxis = 2;

    std::string filename( "!"+in_filename );
    pFits.reset( new FITS( filename , FLOAT_IMG , naxis, naxes ) );

    //PHDU& hdu = pFits->pHDU();
  }
  //  FITS file is now written

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );

  image im( 1, argv_test );

  free( argv_test[0] );
  free( argv_test );

  im.infilename = in_filename;

  BOOST_CHECK_EQUAL( im.read_file(), 0 );

  BOOST_CHECK_EQUAL( im.image_width, naxes[0] );
  BOOST_CHECK_EQUAL( im.image_height, naxes[1] );

  im.infilename = "";
  BOOST_CHECK_EQUAL( im.read_file(), 1 );

  unlink( in_filename.c_str() );
}

BOOST_AUTO_TEST_CASE( write_test )
{
  std::string out_filename = "image_write_test.fits";

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );

  image im( 1, argv_test );

  im.outfilename = out_filename;
  im.parameters  = std::shared_ptr<params>( new params() );
  im.image_width = 10;
  im.image_height = 10;
  im.image_data = std::valarray<double>( 0.0, im.image_width*im.image_height );
  im.write_file();

  image im2( 1, argv_test );
  im2.infilename = out_filename;
  free( argv_test[0] );
  free( argv_test );

  BOOST_CHECK_EQUAL( im2.read_file(), 0 );
  BOOST_CHECK_EQUAL( im2.image_width, im.image_width );
  BOOST_CHECK_EQUAL( im2.image_height, im.image_height );

  unlink( out_filename.c_str() );
}


BOOST_AUTO_TEST_CASE( correct_units_test1 )
{
  std::string in_filename = "correct_units_test1.fits";

  long naxes[2] = { 10, 10 };

  {
    std::unique_ptr<FITS> pFits;

    int naxis = 2;

    std::valarray<double> image_data( 1.0, naxes[0]*naxes[1] );

    std::string filename( "!"+in_filename );
    pFits.reset( new FITS( filename , FLOAT_IMG , naxis, naxes ) );

    PHDU& hdu = pFits->pHDU();
    hdu.addKey( "BUNIT", "ELECTRONS", "" );

    long  fpixel(1);  // ???? what the heck is this useful ...

    hdu.write( fpixel, naxes[0]*naxes[1], image_data );
  }
  //  FITS file is now written

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );

  image im( 1, argv_test );

  free( argv_test[0] );
  free( argv_test );

  im.infilename = in_filename;

  BOOST_CHECK_EQUAL( im.read_file(), 0 );

  BOOST_CHECK_CLOSE( im.image_data[0], 1.0, 1e-10 );  // correct data read ?

  BOOST_CHECK_EQUAL( im.correct_units(), 0 );

  unlink( in_filename.c_str() );
}

BOOST_AUTO_TEST_CASE( correct_units_test2 )
{
  std::string in_filename = "correct_units_test2.fits";

  long naxes[2] = { 10, 10 };

  double exptime = 123.4;

  {
    std::unique_ptr<FITS> pFits;

    int naxis = 2;

    std::valarray<double> image_data( 1.0, naxes[0]*naxes[1] );

    std::string filename( "!"+in_filename );
    pFits.reset( new FITS( filename , FLOAT_IMG , naxis, naxes ) );

    PHDU& hdu = pFits->pHDU();
    hdu.addKey( "BUNIT", "ELECTRONS/S", "" );
    hdu.addKey( "EXPTIME", exptime, "" );

    long  fpixel(1);  // ???? what the heck is this useful ...

    hdu.write( fpixel, naxes[0]*naxes[1], image_data );
  }
  //  FITS file is now written

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );

  image im( 1, argv_test );

  free( argv_test[0] );
  free( argv_test );

  im.infilename = in_filename;

  BOOST_CHECK_EQUAL( im.read_file(), 0 );

  BOOST_CHECK_CLOSE( im.image_data[0], 1.0, 1e-10 );  // correct data read ?

  BOOST_CHECK_EQUAL( im.correct_units(), 0 );

  BOOST_CHECK_CLOSE( im.image_data[0], exptime, 1e-10 );

  unlink( in_filename.c_str() );
}

BOOST_AUTO_TEST_CASE( correct_units_test3 )
{
  std::string in_filename = "correct_units_test3.fits";

  long naxes[2] = { 10, 10 };

  double exptime = 123.4;

  {
    std::unique_ptr<FITS> pFits;

    int naxis = 2;

    std::valarray<double> image_data( 1.0, naxes[0]*naxes[1] );

    std::string filename( "!"+in_filename );
    pFits.reset( new FITS( filename , FLOAT_IMG , naxis, naxes ) );

    PHDU& hdu = pFits->pHDU();
    hdu.addKey( "BUNIT", "ELECTRONS/S", "" );
    hdu.addKey( "MEANEXP", exptime, "" );

    long  fpixel(1);  // ???? what the heck is this useful ...

    hdu.write( fpixel, naxes[0]*naxes[1], image_data );
  }
  //  FITS file is now written

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );

  image im( 1, argv_test );

  free( argv_test[0] );
  free( argv_test );

  im.infilename = in_filename;

  BOOST_CHECK_EQUAL( im.read_file(), 0 );

  BOOST_CHECK_CLOSE( im.image_data[0], 1.0, 1e-10);  // correct data read ?

  im.sci_mode_dark = true;
  BOOST_CHECK_EQUAL( im.correct_units(), 0 );

  BOOST_CHECK_CLOSE( im.image_data[0], exptime ,1e-10 );

  unlink( in_filename.c_str() );
}

BOOST_AUTO_TEST_CASE( correct_units_test4 )
{
  std::string in_filename = "correct_units_test4.fits";

  long naxes[2] = { 10, 10 };

  {
    std::unique_ptr<FITS> pFits;

    int naxis = 2;

    std::valarray<double> image_data( 1.0, naxes[0]*naxes[1] );

    std::string filename( "!"+in_filename );
    pFits.reset( new FITS( filename , FLOAT_IMG , naxis, naxes ) );

    PHDU& hdu = pFits->pHDU();
    hdu.addKey( "BUNIT", "COUNTS", "" );

    long  fpixel(1);  // ???? what the heck is this useful ...

    hdu.write( fpixel, naxes[0]*naxes[1], image_data );
  }
  //  FITS file is now written

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );

  image im( 1, argv_test );

  free( argv_test[0] );
  free( argv_test );

  im.infilename = in_filename;

  BOOST_CHECK_EQUAL( im.read_file(), 0 );

  BOOST_CHECK_CLOSE( im.image_data[0], 1.0, 1e-10 );  // correct data read ?

  BOOST_CHECK_EQUAL( im.correct_units(), 1 );

  unlink( in_filename.c_str() );
}

// given ELECTRONS/S in header but the exptime or mexptime is missing
// asume expteim of 1s

BOOST_AUTO_TEST_CASE( correct_units_test5 )
{
  std::string in_filename = "correct_units_test5.fits";

  long naxes[2] = { 10, 10 };

  {
    std::unique_ptr<FITS> pFits;

    int naxis = 2;

    std::valarray<double> image_data( 1.0, naxes[0]*naxes[1] );

    std::string filename( "!"+in_filename );
    pFits.reset( new FITS( filename , FLOAT_IMG , naxis, naxes ) );

    PHDU& hdu = pFits->pHDU();
    hdu.addKey( "BUNIT", "ELECTRONS/S", "" );


    long  fpixel(1);  // ???? what the heck is this useful ...

    hdu.write( fpixel, naxes[0]*naxes[1], image_data );
  }
  //  FITS file is now written

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );

  image im( 1, argv_test );

  free( argv_test[0] );
  free( argv_test );

  im.infilename = in_filename;

  BOOST_CHECK_EQUAL( im.read_file(), 0 );

  BOOST_CHECK_CLOSE( im.image_data[0], 1.0, 1e-10 );  // correct data read ?

  im.sci_mode_dark = true;
  BOOST_CHECK_EQUAL( im.correct_units(), 0 );

  //bool b = fabs( im.image_data[0] ) < 1e-14;
  //BOOST_CHECK_EQUAL( b, true );
  BOOST_CHECK_CLOSE( im.image_data[0], 1.0, 1e-10 );

  unlink( in_filename.c_str() );
}

// should be the öast test, since we change the current working directory
BOOST_AUTO_TEST_CASE( get_working_path_test )
{
  // first try to use /tmp but on MacOS failed since /tmp -> /private/tmp

  int erg = chdir( "/usr");
  if ( erg == 0 )
  {
    BOOST_CHECK_EQUAL( get_working_path(), "/usr" );
  }
}


BOOST_AUTO_TEST_SUITE_END()
