//#define BOOST_TEST_MODULE strstr

#include <boost/test/unit_test.hpp>

#include "cte_image_neo.hh"
#include "params.hh"
#include "params_acs.hh"

// written by: Oliver Cordes 2017-06-01
// changed by: Oliver Cordes 2017-07-03


BOOST_AUTO_TEST_SUITE( cte_mage_neo_test_suite )

BOOST_AUTO_TEST_CASE( constructor )
{
  cte_image_neo im;

  //BOOST_CHECK_EQUAL( im.prgname, "Program_name_test" );
}


BOOST_AUTO_TEST_CASE( correction1 )
{
   std::shared_ptr<params_acs> p = std::shared_ptr<params_acs>( new params_acs() );

   double jd = +2452334.5 + 3506.238668981474;

   p->calc_trap_config( jd );

   cte_image_neo *im;

   std::shared_ptr<params> p2 = p;
   im = new cte_image_neo( p2 );

   std::valarray<double> in_data =  { 40.787811279, 42.996334076, 36.101192474,
                                      34.036590576, 28.182380676, 33.998012543,
                                      16.299722672, 32.821144104, 30.215837479,
                                      30.543056488 };
   std::valarray<double> out_data = { 40.837474823, 43.048473358, 36.146598816,
                                      34.080699921, 28.220380783, 34.049839020,
                                      16.316484451, 32.891876221, 30.255695343,
                                      30.585535049 };

   im->setup( 1, 10 );

   im->clock_charge( in_data );

   delete im;
}


BOOST_AUTO_TEST_SUITE_END()
