#define BOOST_TEST_MODULE MyTest

#include <boost/test/unit_test.hpp>

#include "image_slice.hh"

// written by: Ole Marggraf 2017-??-??
// changed by: Oliver Cordes 2017-05-29


BOOST_AUTO_TEST_SUITE( image_slice_test_suite )

// check the arithmetics
BOOST_AUTO_TEST_CASE( imageslice_test1 )
{
  // image_slice(width, height, offset, rotation, direction)
  // just give it some arbitrary dimension
  std::image_slice is_yf(13, 8, 5, image_readout_y, image_forward);
  std::image_slice is_yf2;

  // initialized, but no reset() yet... -> pos = begin_pos = end_pos = 0
  BOOST_CHECK_EQUAL(*is_yf, 0);
  is_yf.begin();
  BOOST_CHECK_EQUAL(*is_yf, 0);
  is_yf.end();
  BOOST_CHECK_EQUAL(*is_yf, 0);

  // now do the proper initialization
  // image_slice::reset(col)
  is_yf.reset(0);
  // pos = begin = (height-1-offs)*width + col
  BOOST_CHECK_EQUAL(*is_yf, 65);
  is_yf.reset(5);
  BOOST_CHECK_EQUAL(*is_yf.begin(), 70);
  BOOST_CHECK_EQUAL(*is_yf.end(), 109);
  is_yf++;
  BOOST_CHECK_EQUAL(*is_yf, 83);
  ++is_yf;
  BOOST_CHECK_EQUAL(*is_yf, 96);

  // check not-equal operator
  is_yf2 = is_yf;
  BOOST_CHECK((is_yf!=is_yf2) == false);
  is_yf2++;
  BOOST_CHECK(is_yf!=is_yf2);

  // checking some more begin/end positions for readout variations
  std::image_slice is_yr(13, 8, 5, image_readout_y, image_reverse);

  is_yr.reset(2);
  BOOST_CHECK_EQUAL(*is_yr.begin(), 28);
  BOOST_CHECK_EQUAL(*is_yr.end(), -11);
  is_yr++;
  BOOST_CHECK_EQUAL(*is_yr, 15);
  ++is_yr;
  BOOST_CHECK_EQUAL(*is_yr, 2);

  //
  std::image_slice is_xf(13, 8, 5, image_readout_x, image_forward);

  is_xf.reset(3);
  BOOST_CHECK_EQUAL(*is_xf.begin(), 44);
  BOOST_CHECK_EQUAL(*is_xf.end(), 52);
  is_xf++;
  BOOST_CHECK_EQUAL(*is_xf, 45);
  ++is_xf;
  BOOST_CHECK_EQUAL(*is_xf, 46);

  //
  std::image_slice is_xr(13, 8, 5, image_readout_x, image_reverse);

  is_xr.reset(1);
  BOOST_CHECK_EQUAL(*is_xr.begin(), 20);
  BOOST_CHECK_EQUAL(*is_xr.end(), 12);
  is_xr++;
  BOOST_CHECK_EQUAL(*is_xr, 19);
  ++is_xr;
  BOOST_CHECK_EQUAL(*is_xr, 18);
}

// check whether exceptions are thrown properly
BOOST_AUTO_TEST_CASE( imageslice_exceptions )
{
  // Test the setup configuration and exceptions

  // image_slice(width, height, offset, rotation, direction)
  std::image_slice is_y_ok(20, 10, 0, image_readout_y, image_forward);
  std::image_slice is_y(20, 10, 10, image_readout_y, image_forward);
  std::image_slice is_x(10, 20, 10, image_readout_x, image_forward);

  // throwing exceptions here does not depend on forward or reverse readout
  // reset col parameter is irrelevant because the initialization is set already
  // such that we are out of bounds
  BOOST_CHECK_NO_THROW( is_y_ok.reset(0) );
  BOOST_CHECK_THROW( is_y.reset(0), std::runtime_error );
  BOOST_CHECK_THROW( is_x.reset(0), std::runtime_error );


  // test the out of bounds exceptins with the ++i and i++ operators

  std::image_slice is_forward_out1( 10, 1, 0, image_readout_y, image_forward );
  is_forward_out1.reset( 0 );
  ++is_forward_out1;
  BOOST_CHECK_THROW( ++is_forward_out1, std::runtime_error );

  std::image_slice is_forward_out2( 10, 1, 0, image_readout_y, image_forward );
  is_forward_out2.reset( 0 );
  is_forward_out2++;
  BOOST_CHECK_THROW( is_forward_out2++, std::runtime_error );

  std::image_slice is_reverse_out1( 10, 1, 0, image_readout_y, image_reverse );
  is_reverse_out1.reset( 0 );
  ++is_reverse_out1;
  BOOST_CHECK_THROW( ++is_reverse_out1, std::runtime_error );

  std::image_slice is_reverse_out2( 10, 1, 0, image_readout_y, image_reverse );
  is_reverse_out2.reset( 0 );
  is_reverse_out2++;
  BOOST_CHECK_THROW( is_reverse_out2++, std::runtime_error );
}

BOOST_AUTO_TEST_SUITE_END()
