//#define BOOST_TEST_MODULE strstr

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <iomanip>

#include "cte_image.hh"
#include "output.hh"

// written by: Oliver Cordes 2017-06-01
// changed by: Oliver Cordes 2017-07-19



bool check_array( std::valarray<double> & a, std::valarray<double> & b, double limits )
{
  for (unsigned int i=0; i<a.size();++i)
  {
    if ( fabs( a[i] - b[i] ) > limits )
    {
      std::cout << std::fixed << std::setprecision(9)
                << "Error in array [" << i << "]: a=" << a[i] <<  " b=" << b[i]
                << " diff=" << fabs( a[i] - b[i] ) << " limits=" << limits << std::endl;
      return false;
    }
  }
  return true;
}


BOOST_AUTO_TEST_SUITE( cte_mage_test_suite )

BOOST_AUTO_TEST_CASE( constructor )
{
  cte_image im;

  im.setup( 12,34);
  BOOST_CHECK_EQUAL( im.image_width, 12 );
  BOOST_CHECK_EQUAL( im.image_height, 34 );
}


// test the non used standard algorithm without doing something
BOOST_AUTO_TEST_CASE( clock_charge_test )
{
  cte_image im;

  std::valarray<double> in_data =  { 40.787811279, 42.996334076, 36.101192474,
                                      34.036590576, 28.182380676, 33.998012543,
                                      16.299722672, 32.821144104, 30.215837479,
                                      30.543056488 };
  std::valarray<double> out_data =  { 40.787811279, 42.996334076, 36.101192474,
                                      34.036590576, 28.182380676, 33.998012543,
                                      16.299722672, 32.821144104, 30.215837479,
                                      30.543056488 };

  debug_level = 100;
  im.setup( 1, in_data.size() );
  im.clock_charge( in_data );
  debug_level = 0;

  BOOST_CHECK_EQUAL( check_array( in_data, out_data, 1e-5), true );
}


BOOST_AUTO_TEST_SUITE_END()
