#define BOOST_TEST_MODULE MyTest

#include <boost/test/unit_test.hpp>

#include "params_euclid.hh"

// written by: Oliver Cordes 2017-05-30
// changed by: Oliver Cordes 2017-05-30

BOOST_AUTO_TEST_SUITE( params_euclid_test_suite )

BOOST_AUTO_TEST_CASE( constructor_destructor_test )
{
  params_euclid p;

  BOOST_CHECK_EQUAL( p.working_mode, "EUCLID" );
}

BOOST_AUTO_TEST_CASE( parameter_setting_test )
{
  params_euclid p;

  char **argv_test;
  int    argc;

  argv_test = (char**) malloc( sizeof( void * ) * 2);
  argv_test[0] = strdup( "--hallo" );
  argc = 1;

  p.set_args( &argc, &argv_test );
  free( argv_test[0] );

  BOOST_CHECK_EQUAL( p.has_local_n_species, false );

  free( argv_test );

  std::string key, val;
  int error;

  key = "CCD";
  val = "1";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.ccd, 1 );

  key = "N_SPECIES";
  val = "";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.has_local_n_species, true);

  key = "TRAP_DENSITY";
  val = "";
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.has_local_trap_density, true);
}


BOOST_AUTO_TEST_SUITE_END()
