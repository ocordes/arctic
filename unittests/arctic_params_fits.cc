#define BOOST_TEST_MODULE MyTest

#include <boost/test/unit_test.hpp>

#include "params_fits.hh"

// written by: Oliver Cordes 2017-05-30
// changed by: Oliver Cordes 2017-05-31


BOOST_AUTO_TEST_SUITE( params_fits_test_suite )

BOOST_AUTO_TEST_CASE( constructor_destructor_test )
{
  params_fits p;

  BOOST_CHECK_EQUAL( p.working_mode, "FITS" );
}

BOOST_AUTO_TEST_CASE( parameter_setting_test0 )
{
  params_fits p;

  std::string key, val;
  int error;

  // generel error detecting
  key = "UNKNOWN";
  val = "";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( error, PARSE_ERROR );
}

BOOST_AUTO_TEST_CASE( parameter_setting_test1 )
{
  params_fits p;

  std::string key, val;
  int error;

  key = "XRANGE";
  val = "1,2";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.start_x, 0 );
  BOOST_CHECK_EQUAL( p.end_x, 2 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "YRANGE";
  val = "10,100";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.start_y, 9 );
  BOOST_CHECK_EQUAL( p.end_y, 100 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "READOUT_OFFSET";
  val = "10";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.readout_offset, 10 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );
}

BOOST_AUTO_TEST_CASE( parameter_setting_test2 )
{
  params_fits p;

  std::string key, val;
  int error;

  key = "WELL_DEPTH";
  val = "84700.0";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.well_depth, 84700.0 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "WELL_NOTCH_DEPTH";
  val = "1e-9";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.well_notch_depth, 1e-9 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "WELL_FILL_POWER";
  val = "0.465";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.well_fill_power, 0.465 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "EMPTY_TRAP_LIMIT";
  val = "1e-14";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.empty_trap_limit, 1e-14 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "CHECK_EMPTY_TRAPS";
  val = "";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.check_empty_traps, true );
  BOOST_CHECK_EQUAL( error, PARSE_OK );
}

BOOST_AUTO_TEST_CASE( parameter_setting_test3 )
{
  params_fits p;

  std::string key, val;
  int error;

  key = "N_ITERATIONS";
  val = "1";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.n_iterations, 1 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "EXPRESS";
  val = "5";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.express, 5 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "INJECTION";
  val = "";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.charge_injection, true );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "CLOCK";
  val = "";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.unclock, false );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "UNCLOCK";
  val = "Y";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.unclock, true );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "CUT_UPPER_LIMIT";
  val = "Y";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.cut_upper_limit, true );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "UPPER_LIMIT";
  val = "65536.0";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.upper_limit, 65536.0 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );
}

BOOST_AUTO_TEST_CASE( parameter_setting_test4 )
{
  params_fits p;

  std::string key, val;
  int error;

  key = "CLASSIC";
  val = "Y";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.algorithm, ALGORITHM_CLASSIC );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "CLASSIC";
  val = "N";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.algorithm, ALGORITHM_NEO );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "NEO";
  val = "Y";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.algorithm, ALGORITHM_NEO );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "NEO";
  val = "N";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.algorithm, ALGORITHM_CLASSIC );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "NEO2";
  val = "Y";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.algorithm, ALGORITHM_NEO2 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "NEO2";
  val = "N";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.algorithm, ALGORITHM_NEO );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "DARK_MODE";
  val = "Y";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.dark_mode, true );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "DARK_MODE";
  val = "N";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.dark_mode, false );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "DARK_MODE";
  val = "";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.dark_mode, true );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "NEO2_SPLIT_LIMIT";
  val = "1e-14";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.neo2_split_limit, 1e-14 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );
}

BOOST_AUTO_TEST_CASE( parameter_setting_test5 )
{
  params_fits p;

  std::string key, val;
  int error;
  bool is_equal;

  key = "N_SPECIES";
  val = "3";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.n_species, 3 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "TRAP_DENSITY";
  val = "0.1, 0.05, 1.234";
  error = PARSE_ERROR;
  std::valarray<double> res_trap_density {0.1, 0.05, 1.234};
  p.parse_args( key, val, error );
  std::valarray<bool> a1 = p.trap_density == res_trap_density;
  is_equal = std::all_of(begin(a1), end(a1), [](bool b){return b;});
  BOOST_CHECK_EQUAL( is_equal, true );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "TRAP_LIFETIME";
  val = "0.01, 0.1234, 0.87654";
  error = PARSE_ERROR;
  std::valarray<double> res_trap_lifetime {0.01, 0.1234, 0.07654};
  p.parse_args( key, val, error );
  std::valarray<bool> a2 = p.trap_lifetime == res_trap_lifetime;
  is_equal = std::all_of(begin(a1), end(a1), [](bool b){return b;});
  BOOST_CHECK_EQUAL( is_equal, true );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "N_LEVELS";
  val = "10000";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.n_levels, 10000 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );
}

BOOST_AUTO_TEST_CASE( parameter_setting_test6 )
{
  params_fits p;

  std::string key, val;
  int error;

  key = "ROTATE";
  val = "";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.rotate, image_readout_x );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.rotate, image_readout_y );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "READOUT_X";
  val = "";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.rotate, image_readout_x );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "SERIAL";
  val = "";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.rotate, image_readout_x );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "READOUT_Y";
  val = "";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.rotate, image_readout_y );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "PARALLEL";
  val = "";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.rotate, image_readout_y );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "FORWARD";
  val = "";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.direction, image_forward );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "REVERSE";
  val = "";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.direction, image_reverse );
  BOOST_CHECK_EQUAL( error, PARSE_OK );
}

BOOST_AUTO_TEST_SUITE_END()