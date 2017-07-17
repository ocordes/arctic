//#define BOOST_TEST_MODULE strstr

#include <boost/test/unit_test.hpp>

#include "image.hh"
#include "image_slice.hh"
#include "params.hh"
#include "params_fits.hh"

#include <CCfits/CCfits>

#include <unistd.h>
#include <math.h>

#include <iostream>
#include <fstream>

using namespace CCfits;

// written by: Oliver Cordes 2017-05-31
// changed by: Oliver Cordes 2017-07-14


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


// write test simply to check the header
// - parallel clocking
// - forward clocking
BOOST_AUTO_TEST_CASE( write_test1 )
{
  std::string out_filename = "image_write1_test.fits";

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );

  image im( 1, argv_test );

  im.outfilename = out_filename;
  im.parameters  = std::shared_ptr<params>( new params() );
  im.image_width = 10;
  im.image_height = 10;
  im.image_data = std::valarray<double>( 0.0, im.image_width*im.image_height );
  BOOST_CHECK_EQUAL( im.write_file(), 0 );

  image im2( 1, argv_test );
  im2.infilename = out_filename;
  free( argv_test[0] );
  free( argv_test );

  BOOST_CHECK_EQUAL( im2.read_file(), 0 );
  BOOST_CHECK_EQUAL( im2.image_width, im.image_width );
  BOOST_CHECK_EQUAL( im2.image_height, im.image_height );

  BOOST_CHECK_EQUAL( im2.readkey<std::string>( "CTE_READ", "HALLO" ), "PARALLEL" );
  BOOST_CHECK_EQUAL( im2.readkey<std::string>( "CTE_DIR", "HALLO" ), "FORWARD" );

  unlink( out_filename.c_str() );
}

// write test simply to check the header
// - parallel clocking
// - reverse clocking
BOOST_AUTO_TEST_CASE( write_test2 )
{
  std::string out_filename = "image_write2_test.fits";

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );

  image im( 1, argv_test );

  im.outfilename = out_filename;
  im.parameters  = std::shared_ptr<params>( new params() );
  im.image_width = 10;
  im.image_height = 10;
  im.parameters->direction = image_reverse;
  im.image_data = std::valarray<double>( 0.0, im.image_width*im.image_height );
  BOOST_CHECK_EQUAL( im.write_file(), 0 );

  image im2( 1, argv_test );
  im2.infilename = out_filename;
  free( argv_test[0] );
  free( argv_test );

  BOOST_CHECK_EQUAL( im2.read_file(), 0 );
  BOOST_CHECK_EQUAL( im2.image_width, im.image_width );
  BOOST_CHECK_EQUAL( im2.image_height, im.image_height );

  BOOST_CHECK_EQUAL( im2.readkey<std::string>( "CTE_READ", "HALLO" ), "PARALLEL" );
  BOOST_CHECK_EQUAL( im2.readkey<std::string>( "CTE_DIR", "HALLO" ), "REVERSE" );

  unlink( out_filename.c_str() );
}

// write test simply to check the header
// - serial clocking
// - forward clocking
BOOST_AUTO_TEST_CASE( write_test3 )
{
  std::string out_filename = "image_write3_test.fits";

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );

  image im( 1, argv_test );

  im.outfilename = out_filename;
  im.parameters  = std::shared_ptr<params>( new params() );
  im.image_width = 10;
  im.image_height = 10;
  im.parameters->rotate = image_readout_x;
  im.image_data = std::valarray<double>( 0.0, im.image_width*im.image_height );
  BOOST_CHECK_EQUAL( im.write_file(), 0 );

  image im2( 1, argv_test );
  im2.infilename = out_filename;
  free( argv_test[0] );
  free( argv_test );

  BOOST_CHECK_EQUAL( im2.read_file(), 0 );
  BOOST_CHECK_EQUAL( im2.image_width, im.image_width );
  BOOST_CHECK_EQUAL( im2.image_height, im.image_height );

  BOOST_CHECK_EQUAL( im2.readkey<std::string>( "CTE_READ", "HALLO" ), "SERIAL" );
  BOOST_CHECK_EQUAL( im2.readkey<std::string>( "CTE_DIR", "HALLO" ), "FORWARD" );

  unlink( out_filename.c_str() );
}

// write test simply to check the header
// - serial clocking
// - reverse clocking
BOOST_AUTO_TEST_CASE( write_test4 )
{
  std::string out_filename = "image_write4_test.fits";

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );

  image im( 1, argv_test );

  im.outfilename = out_filename;
  im.parameters  = std::shared_ptr<params>( new params() );
  im.image_width = 10;
  im.image_height = 10;
  im.parameters->rotate = image_readout_x;
  im.parameters->direction = image_reverse;
  im.image_data = std::valarray<double>( 0.0, im.image_width*im.image_height );
  BOOST_CHECK_EQUAL( im.write_file(), 0 );

  image im2( 1, argv_test );
  im2.infilename = out_filename;
  free( argv_test[0] );
  free( argv_test );

  BOOST_CHECK_EQUAL( im2.read_file(), 0 );
  BOOST_CHECK_EQUAL( im2.image_width, im.image_width );
  BOOST_CHECK_EQUAL( im2.image_height, im.image_height );

  BOOST_CHECK_EQUAL( im2.readkey<std::string>( "CTE_READ", "HALLO" ), "SERIAL" );
  BOOST_CHECK_EQUAL( im2.readkey<std::string>( "CTE_DIR", "HALLO" ), "REVERSE" );

  unlink( out_filename.c_str() );
}

// write test can't create file
BOOST_AUTO_TEST_CASE( write_test5 )
{
  std::string out_filename = "/usr/image_write5_test.fits";

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );

  image im( 1, argv_test );

  im.outfilename = out_filename;
  im.parameters  = std::shared_ptr<params>( new params() );
  im.image_width = 10;
  im.image_height = 10;
  im.image_data = std::valarray<double>( 0.0, im.image_width*im.image_height );
  BOOST_CHECK_EQUAL( im.write_file(), 1 );

  free( argv_test[0] );
  free( argv_test );
}

// write test including cpnfig file
BOOST_AUTO_TEST_CASE( write_test6 )
{
  std::string filename = "config_test1.conf";
  std::ofstream f( filename, std::ios::out );
  if ( f.is_open() )
  {
    f << "# Test config file" << std::endl;
    f << "WELL_DEPTH2=100.0" << std::endl;
  }
  else
  {
    BOOST_CHECK( false );
  }
  f.close();

  std::shared_ptr<params_fits> p = std::shared_ptr<params_fits>( new params_fits() );

  p->well_depth = 1234.5;
  p->load_config( filename );

  unlink( filename.c_str() );

  // parameter set including config file

  std::string out_filename = "image_write4_test.fits";

  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );

  image im( 1, argv_test );

  im.outfilename = out_filename;
  im.parameters  = (std::shared_ptr<params>) p;
  im.image_width = 10;
  im.image_height = 10;
  im.parameters->rotate = image_readout_x;
  im.parameters->direction = image_reverse;
  im.image_data = std::valarray<double>( 0.0, im.image_width*im.image_height );
  BOOST_CHECK_EQUAL( im.write_file(), 0 );

  image im2( 1, argv_test );
  im2.infilename = out_filename;
  free( argv_test[0] );
  free( argv_test );

  BOOST_CHECK_EQUAL( im2.read_file(), 0 );
  BOOST_CHECK_EQUAL( im2.readkey<double>( "CTE_WELD", -1.234 ), 1234.5 );
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
