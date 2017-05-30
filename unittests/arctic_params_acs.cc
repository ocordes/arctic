#define BOOST_TEST_MODULE MyTest

#include <boost/test/unit_test.hpp>

#include "params_acs.hh"

// written by: Oliver Cordes 2017-05-30
// changed by: Oliver Cordes 2017-05-30

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

  key = "CCD";
  val = "1";
  //p.parse_args( key, val, error );
  //BOOST_CHECK_EQUAL( p.ccd, 1 );
}


BOOST_AUTO_TEST_SUITE_END()
