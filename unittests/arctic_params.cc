#define BOOST_TEST_MODULE MyTest

#include <boost/test/unit_test.hpp>

#include "params.hh"

// written by: Oliver Cordes 2017-01-06
// chnaged by: Oliver Cordes 2017-07-13


BOOST_AUTO_TEST_SUITE( params_test_suite )

BOOST_AUTO_TEST_CASE( params_test_str2array )
{
  params p;

  std::string test_str1 = "10.3,28.7253212,-0.00001";
  std::valarray<double> res_array1 {10.3, 28.7253212, -0.00001};

  std::string test_str2 = "";
  std::valarray<double> res_array2 {};

  std::string test_str3 = "3.1415";
  std::valarray<double> res_array3 {3.1415};

  std::string test_str4 = ",-00.01,";
  std::valarray<double> res_array4 {-0.01};

  std::string test_str5 = ".007";
  std::valarray<double> res_array5 {0.007};

  // we cannot evaluate std::valarray with boost directly, so project it down into a single bool
  std::valarray<bool> a1 = p.str2array(test_str1) == res_array1;
  bool is_equal1 = std::all_of(begin(a1), end(a1), [](bool b){return b;});

  std::valarray<bool> a2 = p.str2array(test_str2) == res_array2;
  bool is_equal2 = std::all_of(begin(a2), end(a2), [](bool b){return b;});

  std::valarray<bool> a3 = p.str2array(test_str3) == res_array3;
  bool is_equal3 = std::all_of(begin(a3), end(a3), [](bool b){return b;});

  std::valarray<bool> a4 = p.str2array(test_str4) == res_array4;
  bool is_equal4 = std::all_of(begin(a4), end(a4), [](bool b){return b;});

  std::valarray<bool> a5 = p.str2array(test_str5) == res_array5;
  bool is_equal5 = std::all_of(begin(a5), end(a5), [](bool b){return b;});

  BOOST_CHECK_EQUAL(is_equal1, true);
  BOOST_CHECK_EQUAL(is_equal2, true);
  BOOST_CHECK_EQUAL(is_equal3, true);
  BOOST_CHECK_EQUAL(is_equal4, true);
  BOOST_CHECK_EQUAL(is_equal5, true);

}


BOOST_AUTO_TEST_CASE( get_working_mode_test )
{
  char **argv_test;

  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[1] = strdup( "-m" );
  argv_test[0] = strdup( "Foo" );

  argv_test[2] = strdup( "FITS" );
  BOOST_CHECK_EQUAL( get_working_mode( 3, argv_test ), WORKING_MODE_FITS );
  free( argv_test[2] );

  argv_test[2] = strdup( "ACS" );
  BOOST_CHECK_EQUAL( get_working_mode( 3, argv_test ), WORKING_MODE_ACS );
  free( argv_test[2] );

  argv_test[2] = strdup( "EUCLID" );
  BOOST_CHECK_EQUAL( get_working_mode( 3, argv_test ), WORKING_MODE_EUCLID );
  free( argv_test[2] );

  argv_test[2] = strdup( "BAR" );
  BOOST_CHECK_EQUAL( get_working_mode( 3, argv_test ), WORKING_MODE_FITS );
  free( argv_test[2] );

  // no argument given
  BOOST_CHECK_EQUAL( get_working_mode( 2, argv_test ), WORKING_MODE_FITS );

  free( argv_test[1] );
  free( argv_test[0] );

  free( argv_test );
}

// check the tobool function
BOOST_AUTO_TEST_CASE( tobool_test )
{
  params p;

  std::string s = "";
  int error;
  BOOST_CHECK_EQUAL( p.tobool( s, true, error ), true );
  BOOST_CHECK_EQUAL( error, PARSE_UNKNOWN );
  BOOST_CHECK_EQUAL( p.tobool( s, false, error ), false);
  BOOST_CHECK_EQUAL( error, PARSE_UNKNOWN );
  s = "Y";
  BOOST_CHECK_EQUAL( p.tobool( s, false, error ), true );
  BOOST_CHECK_EQUAL( error, PARSE_OK );
  s = "J";
  BOOST_CHECK_EQUAL( p.tobool( s, false, error ), true );
  BOOST_CHECK_EQUAL( error, PARSE_OK );
  s = "T";
  BOOST_CHECK_EQUAL( p.tobool( s, false, error ), true );
  BOOST_CHECK_EQUAL( error, PARSE_OK );
  s = "1";
  BOOST_CHECK_EQUAL( p.tobool( s, false, error ), true );
  BOOST_CHECK_EQUAL( error, PARSE_OK );
  s = "N";
  BOOST_CHECK_EQUAL( p.tobool( s, true, error ), false );
  BOOST_CHECK_EQUAL( error, PARSE_OK );
  s = "F";
  BOOST_CHECK_EQUAL( p.tobool( s, true, error ), false );
  BOOST_CHECK_EQUAL( error, PARSE_OK );
  s = "0";
  BOOST_CHECK_EQUAL( p.tobool( s, true, error ), false );
  BOOST_CHECK_EQUAL( error, PARSE_OK );
  s = "a";
  BOOST_CHECK_EQUAL( p.tobool( s, true, error ), true );
  BOOST_CHECK_EQUAL( error, PARSE_UNKNOWN );
}

// check the val2int rouint
BOOST_AUTO_TEST_CASE( val2int_test )
{
  params p;

  std::string s = "";
  int error;

  p.val2int( s, error );
  BOOST_CHECK_EQUAL( error, PARSE_UNKNOWN );

  s = "1234";
  BOOST_CHECK_EQUAL( p.val2int( s, error ), 1234 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  s = "-1234";
  BOOST_CHECK_EQUAL( p.val2int( s, error ), -1234 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  s = "a";
  BOOST_CHECK_EQUAL( p.val2int( s, error ), 0 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );
}

// check the val2int rouint
BOOST_AUTO_TEST_CASE( val2double_test )
{
  params p;

  std::string s = "";
  int error;

  p.val2double( s, error );
  BOOST_CHECK_EQUAL( error, PARSE_UNKNOWN );

  s = "1234.5";
  BOOST_CHECK_EQUAL( p.val2double( s, error ), 1234.5 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  s = "-1234.5";
  BOOST_CHECK_EQUAL( p.val2double( s, error ), -1234.5 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  s = "a";
  BOOST_CHECK_EQUAL( p.val2double( s, error ), 0.0 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );
}


BOOST_AUTO_TEST_SUITE_END()
