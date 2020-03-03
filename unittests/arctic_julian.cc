//#define BOOST_TEST_MODULE MyTest

#include <boost/test/unit_test.hpp>

#include "julian.hh"


BOOST_AUTO_TEST_SUITE( julian_test_suite )

BOOST_AUTO_TEST_CASE( juliandate_test1 )
{
  /* definition of tolerance e according to Boost docs:
   *
   * condition (very close with tolerance e) is met if
   * |u-v|/|u| <= e and |u-v|/|v| <= e
   *
   * Also, e is given in percent.
   *
   * Hence, we need to compute a reasonable tolerance per individual test.
   */
  BOOST_CHECK_CLOSE( juliandate(1700,1,1,0,0,0), 2341972.5, 1e-10 );
  BOOST_CHECK_CLOSE( juliandate(1858,11,17,0,0,0), 2400000.5, 1e-10 );
  BOOST_CHECK_CLOSE( juliandate(1980,1,1,0,0,1), 2444239.5000116, 1e-10 );
  BOOST_CHECK_CLOSE( juliandate(2035,12,31,23,59,59), 2464693.499988, 1e-10 );
}

BOOST_AUTO_TEST_SUITE_END()
