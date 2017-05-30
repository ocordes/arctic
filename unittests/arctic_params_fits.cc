#define BOOST_TEST_MODULE MyTest

#include <boost/test/unit_test.hpp>

#include "params_fits.hh"

// written by: Oliver Cordes 2017-05-30
// changed by: Oliver Cordes 2017-05-30


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

  key = "UNKNOWN";
  val = "";
  p.parse_args( key, val, error );
  //BOOST_CHECK_EQUAL( error, PARSE_ERROR );
}

BOOST_AUTO_TEST_CASE( parameter_setting_test1 )
{
  params_fits p;

  std::string key, val;
  int error;

  key = "XRANGE";
  val = "1,2";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.start_x, 0 );
  BOOST_CHECK_EQUAL( p.end_x, 2 );

  key = "YRANGE";
  val = "10,100";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.start_y, 9 );
  BOOST_CHECK_EQUAL( p.end_y, 100 );

  key = "READOUT_OFFSET";
  val = "10";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.readout_offset, 10 );
}

BOOST_AUTO_TEST_CASE( parameter_setting_test2 )
{
  params_fits p;

  std::string key, val;
  int error;

  key = "WELL_DEPTH";
  val = "84700.0";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.well_depth, 84700.0 );

  key = "WELL_NOTCH_DEPTH";
  val = "1e-9";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.well_notch_depth, 1e-9 );

  key = "WELL_FILL_POWER";
  val = "0.465";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.well_fill_power, 0.465 );

  key = "EMPTY_TRAP_LIMIT";
  val = "1e-14";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.empty_trap_limit, 1e-14 );

  key = "CHECK_EMPTY_TRAPS";
  val = "";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.check_empty_traps, true );
}

BOOST_AUTO_TEST_CASE( parameter_setting_test3 )
{
  params_fits p;

  std::string key, val;
  int error;

  key = "N_ITERATIONS";
  val = "1";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.n_iterations, 1 );

  key = "EXPRESS";
  val = "5";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.express, 5 );

  key = "INJECTION";
  val = "";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.charge_injection, true );

  key = "CLOCK";
  val = "";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.unclock, false );

  key = "UNCLOCK";
  val = "Y";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.unclock, true );

  key = "CUT_UPPER_LIMIT";
  val = "Y";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.cut_upper_limit, true );

  key = "UPPER_LIMIT";
  val = "65536.0";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.upper_limit, 65536.0 );
}

BOOST_AUTO_TEST_CASE( parameter_setting_test4 )
{
  params_fits p;

  std::string key, val;
  int error;

  key = "CLASSIC";
  val = "Y";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.algorithm, ALGORITHM_CLASSIC );

  key = "CLASSIC";
  val = "N";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.algorithm, ALGORITHM_NEO );

  key = "NEO";
  val = "Y";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.algorithm, ALGORITHM_NEO );

  key = "NEO";
  val = "N";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.algorithm, ALGORITHM_CLASSIC );

  key = "NEO2";
  val = "Y";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.algorithm, ALGORITHM_NEO2 );

  key = "NEO2";
  val = "N";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.algorithm, ALGORITHM_NEO );

  key = "DARK_MODE";
  val = "Y";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.dark_mode, true );

  key = "DARK_MODE";
  val = "N";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.dark_mode, false );

  key = "DARK_MODE";
  val = "";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.dark_mode, true );

  key = "NEO2_SPLIT_LIMIT";
  val = "1e-14";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.neo2_split_limit, 1e-14 );
}

BOOST_AUTO_TEST_CASE( parameter_setting_test5 )
{
  params_fits p;

  std::string key, val;
  int error;
  bool is_equal;

  key = "N_SPECIES";
  val = "3";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.n_species, 3 );

  key = "TRAP_DENSITY";
  val = "0.1, 0.05, 1.234";
  std::valarray<double> res_trap_density {0.1, 0.05, 1.234};
  p.parse_args( key, val, error );
  std::valarray<bool> a1 = p.trap_density == res_trap_density;
  is_equal = std::all_of(begin(a1), end(a1), [](bool b){return b;});
  BOOST_CHECK_EQUAL( is_equal, true );

  key = "TRAP_LIFETIME";
  val = "0.01, 0.1234, 0.87654";
  std::valarray<double> res_trap_lifetime {0.01, 0.1234, 0.07654};
  p.parse_args( key, val, error );
  std::valarray<bool> a2 = p.trap_lifetime == res_trap_lifetime;
  is_equal = std::all_of(begin(a1), end(a1), [](bool b){return b;});
  BOOST_CHECK_EQUAL( is_equal, true );

  key = "N_LEVELS";
  val = "10000";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.n_levels, 10000 );
}

BOOST_AUTO_TEST_CASE( parameter_setting_test6 )
{
  params_fits p;

  std::string key, val;
  int error;

  key = "ROTATE";
  val = "";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.rotate, image_readout_x );

  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.rotate, image_readout_x );

  key = "READOUT_X";
  val = "";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.rotate, image_readout_x );

  key = "SERIAL";
  val = "";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.rotate, image_readout_x );

  key = "READOUT_Y";
  val = "";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.rotate, image_readout_y );

  key = "PARALLEL";
  val = "";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.rotate, image_readout_y );

  key = "FORWARD";
  val = "";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.direction, image_forward );

  key = "REVERSE";
  val = "";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.direction, image_reverse );
}

BOOST_AUTO_TEST_SUITE_END()
