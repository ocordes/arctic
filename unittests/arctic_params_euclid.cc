#define BOOST_TEST_MODULE MyTest

#include <boost/test/unit_test.hpp>

#include "params_euclid.hh"

// written by: Oliver Cordes 2017-05-30
// changed by: Oliver Cordes 2017-05-31

BOOST_AUTO_TEST_SUITE( params_euclid_test_suite )

BOOST_AUTO_TEST_CASE( constructor_destructor_test )
{
  params_euclid p;

  BOOST_CHECK_EQUAL( p.working_mode, "EUCLID" );
}

BOOST_AUTO_TEST_CASE( parameter_setting_test )
{
  params_euclid p1;

  char **argv_test;
  int    argc;

  argv_test = (char**) malloc( sizeof( void * ) * 2);
  argv_test[0] = strdup( "Program_name_test" );
  argc = 1;

  // check zeor parameters
  p1.set_args( &argc, &argv_test );

  BOOST_CHECK_EQUAL( p1.has_local_n_species, false );
  BOOST_CHECK_EQUAL( p1.has_local_trap_density, false );


  // check for local parameter override -> indirect settings
  argv_test[1] = strdup( "--n_species=1" );
  argc = 2;
  p1.set_args( &argc, &argv_test );
  free( argv_test[1] );
  BOOST_CHECK_EQUAL( p1.has_local_n_species, true );

  argv_test[1] = strdup( "--trap_density=0.1,0.2" );
  argc = 2;
  p1.set_args( &argc, &argv_test );
  free( argv_test[1] );
  BOOST_CHECK_EQUAL( p1.has_local_trap_density, true );

  free( argv_test[0] );
  free( argv_test );


  // test for common parameters
  params_euclid p;

  std::string key, val;
  int error;

  key = "CCD";
  val = "1";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.ccd, 1 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  // check direct settings
  key = "N_SPECIES";
  val = "";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.has_local_n_species, true);
  BOOST_CHECK_EQUAL( error, PARSE_OK );

  key = "TRAP_DENSITY";
  val = "";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.has_local_trap_density, true);
  BOOST_CHECK_EQUAL( error, PARSE_OK );
}


BOOST_AUTO_TEST_SUITE_END()
