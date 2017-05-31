#define BOOST_TEST_MODULE MyTest

#include <boost/test/unit_test.hpp>

#include "params_acs.hh"

// written by: Oliver Cordes 2017-05-30
// changed by: Oliver Cordes 2017-05-31

BOOST_AUTO_TEST_SUITE( params_acs_test_suite )

BOOST_AUTO_TEST_CASE( constructor_destructor_test )
{
  params_acs p;

  BOOST_CHECK_EQUAL( p.working_mode, "ACS" );
}

BOOST_AUTO_TEST_CASE( parameter_setting_test )
{
  params_acs p;

  std::string key, val;
  int error;

  // check parameter redirection to params_fits
  key = "XRANGE";
  val = "1,2";
  error = PARSE_ERROR;
  p.parse_args( key, val, error );
  BOOST_CHECK_EQUAL( p.start_x, 0 );
  BOOST_CHECK_EQUAL( p.end_x, 2 );
  BOOST_CHECK_EQUAL( error, PARSE_OK );
}


BOOST_AUTO_TEST_CASE( calc_trap_config_test )
{
  params_acs p;

  double jd;

  double ld = 3506.238668981474;

  jd = ld + 2452334.5; // getthe correct julian day

  p.calc_trap_config( jd );

  BOOST_CHECK_EQUAL( p.n_species, 3 );
  if ( p.n_species == 3 )
  {
    BOOST_CHECK_CLOSE( p.trap_density[0], 0.29331394585796777, 1e-10 );
    BOOST_CHECK_CLOSE( p.trap_density[1], 0.78063293060403316, 1e-10 );
    BOOST_CHECK_CLOSE( p.trap_density[2], 0.6582255049283231, 1e-10 );
    BOOST_CHECK_CLOSE( p.trap_lifetime[0], 0.73999999999999999, 1e-10 );
    BOOST_CHECK_CLOSE( p.trap_lifetime[1], 7.7000000000000002, 1e-10 );
    BOOST_CHECK_CLOSE( p.trap_lifetime[2], 37.00, 1e-10 );
  }


  // check axs parameter before Hubble/ACS was repaired in SM4

  jd = 2454968.0 - 7.0; // one week before repair ...

  p.calc_trap_config( jd );

  BOOST_CHECK_EQUAL( p.n_species, 3 );
  if ( p.n_species == 3 )
  {
    BOOST_CHECK_CLOSE( p.trap_density[0], 0.16085573620756002, 1e-10 );
    BOOST_CHECK_CLOSE( p.trap_density[1], 0.42810540219244003, 1e-10 );
    BOOST_CHECK_CLOSE( p.trap_density[2], 0.36097618160000006, 1e-10 );
    BOOST_CHECK_CLOSE( p.trap_lifetime[0], 0.73999999999999999, 1e-10 );
    BOOST_CHECK_CLOSE( p.trap_lifetime[1], 7.7000000000000002, 1e-10 );
    BOOST_CHECK_CLOSE( p.trap_lifetime[2], 37.00, 1e-10 );
  }
}


BOOST_AUTO_TEST_SUITE_END()
