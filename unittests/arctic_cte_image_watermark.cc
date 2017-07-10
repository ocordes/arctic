//#define BOOST_TEST_MODULE strstr

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <iomanip>

#include "cte_image_watermark.hh"
#include "params.hh"
#include "params_acs.hh"
#include "output.hh"

// written by: Oliver Cordes 2017-06-01
// changed by: Oliver Cordes 2017-06-01


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


BOOST_AUTO_TEST_SUITE( cte_mage_watermark_test_suite )

BOOST_AUTO_TEST_CASE( constructor )
{
  cte_image_watermark im;

  //BOOST_CHECK_EQUAL( im.prgname, "Program_name_test" );
}

// check the following parameters
//  - unclocking
//  - no rotation
//  - express = 1
//  - n_iteration = 1
//  - cut_upper_limits = false
//  - high signal to noise
//  - dark_mode = false
//  - charge_injection = false
BOOST_AUTO_TEST_CASE( correction1 )
{
   std::shared_ptr<params_acs> p = std::shared_ptr<params_acs>( new params_acs() );

   double jd = +2452334.5 + 3506.238668981474;

   p->calc_trap_config( jd );

   cte_image_watermark im( (std::shared_ptr<params> &) p );

   std::valarray<double> in_data =  { 40.787811279, 42.996334076, 36.101192474,
                                      34.036590576, 28.182380676, 33.998012543,
                                      16.299722672, 32.821144104, 30.215837479,
                                      30.543056488 };
   std::valarray<double> out_data = { 40.837473022, 43.048471768, 36.146614489,
                                      34.080822266, 28.220484291, 34.049768026,
                                      16.316589522, 32.891894026, 30.255838231,
                                      30.585432681 };

   im.setup( 1, in_data.size() );
   im.clock_charge( in_data );

   //for (unsigned int i=0;i<in_data.size();++i)
   //     std::cout << std::fixed << std::setprecision(9) << in_data[i] << ", ";
   //std::cout << std::endl;

   BOOST_CHECK_EQUAL( check_array( in_data, out_data, 1e-5), true );
}


// check the following parameters
//  - unclocking
//  - no rotation
//  - direction forward
//  - express = 1
//  - n_iteration = 1
//  - cut_upper_limits = false
//  - low signal to noise
//  - dark_mode = false
//  - charge_injection = false
BOOST_AUTO_TEST_CASE( correction2 )
{
   std::shared_ptr<params_acs> p = std::shared_ptr<params_acs>( new params_acs() );

   //double jd = +2452334.5 + 3506.238668981474;
   double jd = +2452334.5 + 2786.02;

   p->calc_trap_config( jd );

   cte_image_watermark im( (std::shared_ptr<params> &) p );

   std::valarray<double> in_data =  { 0.008679612, 0.002772796, 0.001571912, 0.001605783,
                                      0.000871051, 0.001173347, 0.001275275, 0.000993010,
                                      -0.000350387, 0.003193463, 0.018024899, 0.014778999,
                                      0.002280427, 0.001436972, 0.000917238, 0.006845565,
                                      0.004871908, 0.291465133, 0.003689340, 0.020187818,
                                      0.002238023, 0.003081655, 0.000947794, 0.000270663,
                                      -0.000422476, 0.002363467, 0.001035341, 0.028084122,
                                      0.006980744, 0.003283431, 0.007109892, 0.003935513,
                                      0.000978794, -0.000460752, -0.001094718, 0.001689621,
                                      0.002342759, 0.007524569, 0.000076040, 0.001714738,
                                      0.000504006, 0.007206579, 0.000830590, 0.012597033,
                                      0.002316516, 0.012605761, 0.015178495, 0.003467531,
                                      0.000858165, 0.002558219, 0.002378170, 0.002249520,
                                      0.001153489, -0.000369282, 0.002616879, 0.001682350,
                                      0.002737300, 0.004039197, 0.015030440, 0.000175450,
                                      0.001274341, 0.008631511, 0.004427247, 0.002912365,
                                      0.001941754, 0.002695119, 0.001299986, 0.000986228,
                                      0.000801815, 0.000617397, 0.002942618, 0.026314680,
                                      0.008317295, 0.000036347, 0.014502814, 0.001302069,
                                      0.002505186, 0.067330942, 0.003535634, 0.000450654 };

   std::valarray<double> out_data = { 0.009424694, 0.003164889, 0.001865360, 0.001944310,
                                      0.001087437, 0.001532330, 0.001655542, 0.001273407,
                                      -0.000367015, 0.005296758, 0.024725593, 0.015687481,
                                      0.001731822, 0.001183639, 0.000758222, 0.009297302,
                                      0.005239197, 0.345897190, -0.004963907, 0.018585195,
                                      -0.001635478, 0.000640321, -0.001423332, -0.001810380,
                                      -0.002234126, 0.001979415, -0.000273190, 0.040624039,
                                      0.004742687, 0.001290465, 0.007045717, 0.002771741,
                                      -0.000628943, -0.001937729, -0.002224807, 0.001810590,
                                      0.002604743, 0.011457916, -0.001598655, 0.001408532,
                                      -0.000270167, 0.010977892, -0.000350368, 0.020755314,
                                      0.000819247, 0.016868398, 0.019796339, 0.001656960,
                                      -0.000873127, 0.002276620, 0.002029171, 0.002022631,
                                      0.000650824, -0.001117062, 0.003859376, 0.001699797,
                                      0.003973821, 0.006440337, 0.028123836, -0.002426407,
                                      0.000252848, 0.012618639, 0.004176493, 0.002485431,
                                      0.001507397, 0.002979486, 0.001009650, 0.000825569,
                                      0.000767468, 0.000667599, 0.005397469, 0.058433289,
                                      0.010185970, -0.003083751, 0.020770972, -0.001462083,
                                      0.001405211, 0.129477061, -0.005787341, -0.005975364 };

   im.setup( 1, in_data.size() );
   //debug_level = 10;
   im.clock_charge( in_data );

   //for (unsigned int i=0;i<in_data.size();++i)
   //     std::cout << std::fixed << std::setprecision(9) << in_data[i] << ", ";
   //std::cout << std::endl;

   BOOST_CHECK_EQUAL( check_array( in_data, out_data, 1e-9), true );
}

// check the following parameters
//  - unclocking
//  - no rotation
//  - direction forward
//  - express = 2
//  - n_iteration = 1
//  - cut_upper_limits = false
//  - high signal to noise
//  - dark_mode = false
//  - charge_injection = false
BOOST_AUTO_TEST_CASE( correction3 )
{
   std::shared_ptr<params_acs> p = std::shared_ptr<params_acs>( new params_acs() );

   double jd = +2452334.5 + 3506.238668981474;

   p->calc_trap_config( jd );
   p->express = 2;

   cte_image_watermark im( (std::shared_ptr<params> &) p );

   std::valarray<double> in_data =  { 40.787811279, 42.996334076, 36.101192474,
                                      34.036590576, 28.182380676, 33.998012543,
                                      16.299722672, 32.821144104, 30.215837479,
                                      30.543056488 };
   std::valarray<double> out_data = { 40.837473022, 43.048471768, 36.146614489,
                                      34.080822266, 28.220484291, 34.049731349,
                                      16.316555457, 32.891862039, 30.255801865,
                                      30.585389830 };

   im.setup( 1, in_data.size() );
   im.clock_charge( in_data );

   //for (unsigned int i=0;i<in_data.size();++i)
   // std::cout << std::fixed << std::setprecision(9) << in_data[i] << ", ";
   //  std::cout << std::endl;

   BOOST_CHECK_EQUAL( check_array( in_data, out_data, 1e-5), true );
}


// check the following parameters
//  - unclocking
//  - no rotation
//  - direction forward
//  - express = 1
//  - n_iteration = 2
//  - cut_upper_limits = false
//  - high signal to noise
//  - dark_mode = false
//  - charge_injection = false
BOOST_AUTO_TEST_CASE( correction4 )
{
   std::shared_ptr<params_acs> p = std::shared_ptr<params_acs>( new params_acs() );

   double jd = +2452334.5 + 3506.238668981474;

   p->calc_trap_config( jd );
   p->n_iterations = 2;

   cte_image_watermark im( (std::shared_ptr<params> &) p );

   std::valarray<double> in_data =  { 40.787811279, 42.996334076, 36.101192474,
                                      34.036590576, 28.182380676, 33.998012543,
                                      16.299722672, 32.821144104, 30.215837479,
                                      30.543056488 };
   std::valarray<double> out_data = { 40.837501130, 43.048501035, 36.146641456,
                                      34.080849694, 28.220509467, 34.049812494,
                                      16.316587105, 32.892009596, 30.255842153,
                                      30.585456290 };

   im.setup( 1, in_data.size() );
   im.clock_charge( in_data );

  //  for (unsigned int i=0;i<in_data.size();++i)
  //    std::cout << std::fixed << std::setprecision(9) << in_data[i] << ", ";
  //  std::cout << std::endl;

   BOOST_CHECK_EQUAL( check_array( in_data, out_data, 1e-5), true );
}

// check the following parameters
//  - unclocking
//  - no rotation
//  - direction forward
//  - express = 1
//  - n_iteration = 1
//  - cut_upper_limits = true limits=40.0
//  - high signal to noise
//  - dark_mode = false
//  - charge_injection = false
BOOST_AUTO_TEST_CASE( correction5 )
{
   std::shared_ptr<params_acs> p = std::shared_ptr<params_acs>( new params_acs() );

   double jd = +2452334.5 + 3506.238668981474;

   p->calc_trap_config( jd );
   p->cut_upper_limit = true;
   p->upper_limit = 40.0;

   cte_image_watermark im( (std::shared_ptr<params> &) p );

   std::valarray<double> in_data =  { 40.787811279, 42.996334076, 36.101192474,
                                      34.036590576, 28.182380676, 33.998012543,
                                      16.299722672, 32.821144104, 30.215837479,
                                      30.543056488 };
   std::valarray<double> out_data = { 40.000000000, 40.000000000, 36.146614489,
                                      34.080822266, 28.220484291, 34.049768026,
                                      16.316589522, 32.891894026, 30.255838231,
                                      30.585432681 };

   im.setup( 1, in_data.size() );
   im.clock_charge( in_data );

   //for (unsigned int i=0;i<in_data.size();++i)
   //   std::cout << std::fixed << std::setprecision(9) << in_data[i] << ", ";
   //std::cout << std::endl;

   BOOST_CHECK_EQUAL( check_array( in_data, out_data, 1e-5), true );
}

// check the following parameters
//  - unclocking
//  - rotation,
//  - direction forward
//  - express = 1
//  - n_iteration = 1
//  - cut_upper_limits = false
//  - high signal to noise
//  - dark_mode = false
//  - charge_injection = false
BOOST_AUTO_TEST_CASE( correction6 )
{
   std::shared_ptr<params_acs> p = std::shared_ptr<params_acs>( new params_acs() );

   double jd = +2452334.5 + 3506.238668981474;

   p->calc_trap_config( jd );
   p->rotate = true;

   cte_image_watermark im( (std::shared_ptr<params> &) p );

   std::valarray<double> in_data =  { 40.787811279, 42.996334076, 36.101192474,
                                      34.036590576, 28.182380676, 33.998012543,
                                      16.299722672, 32.821144104, 30.215837479,
                                      30.543056488 };
   std::valarray<double> out_data = { 40.837473022, 43.048471768, 36.146614489,
                                      34.080822266, 28.220484291, 34.049768026,
                                      16.316589522, 32.891894026, 30.255838231,
                                      30.585432681 };

   im.setup( in_data.size(), 1 );
   im.clock_charge( in_data );

   //for (unsigned int i=0;i<in_data.size();++i)
   //   std::cout << std::fixed << std::setprecision(9) << in_data[i] << ", ";
   //std::cout << std::endl;

   BOOST_CHECK_EQUAL( check_array( in_data, out_data, 1e-5), true );
}

// check the following parameters
//  - unclocking
//  - no rotation
//  - direction forward
//  - express = 1
//  - n_iteration = 1
//  - cut_upper_limits = false
//  - low signal to noise
//  - dark_mode = true
//  - charge_injection = false
BOOST_AUTO_TEST_CASE( correction7 )
{
   std::shared_ptr<params_acs> p = std::shared_ptr<params_acs>( new params_acs() );

   //double jd = +2452334.5 + 3506.238668981474;
   double jd = +2452334.5 + 2786.02;

   p->calc_trap_config( jd );
   p->dark_mode = true;

   cte_image_watermark im( (std::shared_ptr<params> &) p );

   std::valarray<double> in_data =  { 0.008679612, 0.002772796, 0.001571912, 0.001605783,
                                      0.000871051, 0.001173347, 0.001275275, 0.000993010,
                                      -0.000350387, 0.003193463, 0.018024899, 0.014778999,
                                      0.002280427, 0.001436972, 0.000917238, 0.006845565,
                                      0.004871908, 0.291465133, 0.003689340, 0.020187818,
                                      0.002238023, 0.003081655, 0.000947794, 0.000270663,
                                      -0.000422476, 0.002363467, 0.001035341, 0.028084122,
                                      0.006980744, 0.003283431, 0.007109892, 0.003935513,
                                      0.000978794, -0.000460752, -0.001094718, 0.001689621,
                                      0.002342759, 0.007524569, 0.000076040, 0.001714738,
                                      0.000504006, 0.007206579, 0.000830590, 0.012597033,
                                      0.002316516, 0.012605761, 0.015178495, 0.003467531,
                                      0.000858165, 0.002558219, 0.002378170, 0.002249520,
                                      0.001153489, -0.000369282, 0.002616879, 0.001682350,
                                      0.002737300, 0.004039197, 0.015030440, 0.000175450,
                                      0.001274341, 0.008631511, 0.004427247, 0.002912365,
                                      0.001941754, 0.002695119, 0.001299986, 0.000986228,
                                      0.000801815, 0.000617397, 0.002942618, 0.026314680,
                                      0.008317295, 0.000036347, 0.014502814, 0.001302069,
                                      0.002505186, 0.067330942, 0.003535634, 0.000450654 };

   std::valarray<double> out_data = { 0.009424694, 0.003164889, 0.001865360, 0.001944310,
                                      0.001087437, 0.001532330, 0.001655542, 0.001273407,
                                      -0.000367015, 0.005296758, 0.024725593, 0.015687481,
                                      0.001731822, 0.001183639, 0.000758222, 0.009297302,
                                      0.005239197, 0.345897190, -0.004963907, 0.018585195,
                                      -0.001635478, 0.000640321, -0.001423332, -0.001810380,
                                      -0.002234126, 0.001979415, -0.000273190, 0.040624039,
                                      0.004742687, 0.001290465, 0.007045717, 0.002771741,
                                      -0.000628943, -0.001937729, -0.002224807, 0.001810590,
                                      0.002604743, 0.011457916, -0.001598655, 0.001408532,
                                      -0.000270167, 0.010977892, -0.000350368, 0.020755314,
                                      0.000819247, 0.016868398, 0.019796339, 0.001656960,
                                      -0.000873127, 0.002276620, 0.002029171, 0.002022631,
                                      0.000650824, -0.001117062, 0.003859376, 0.001699797,
                                      0.003973821, 0.006440337, 0.028123836, -0.002426407,
                                      0.000252848, 0.012618639, 0.004176493, 0.002485431,
                                      0.001507397, 0.002979486, 0.001009650, 0.000825569,
                                      0.000767468, 0.000667599, 0.005397469, 0.058433289,
                                      0.010185970, -0.003083751, 0.020770972, -0.001462083,
                                      0.001405211, 0.129477061, -0.005787341, -0.005975364 };

   im.setup( 1, in_data.size() );

   im.clock_charge( in_data );

   //for (unsigned int i=0;i<in_data.size();++i)
   // std::cout << std::fixed << std::setprecision(9) << in_data[i] << ", ";
   //std::cout << std::endl;

   BOOST_CHECK_EQUAL( check_array( in_data, out_data, 1e-9), true );
}

// check the following parameters
//  - clocking !!
//  - no rotation
//  - direction forward
//  - express = 1
//  - n_iteration = 1
//  - cut_upper_limits = false
//  - high signal to noise
//  - dark_mode = false
//  - charge_injection = false
BOOST_AUTO_TEST_CASE( correction8 )
{
   std::shared_ptr<params_acs> p = std::shared_ptr<params_acs>( new params_acs() );

   double jd = +2452334.5 + 3506.238668981474;

   p->calc_trap_config( jd );
   p->unclock = false;

   cte_image_watermark im( (std::shared_ptr<params> &) p );

   std::valarray<double> in_data =  { 40.787811279, 42.996334076, 36.101192474,
                                      34.036590576, 28.182380676, 33.998012543,
                                      16.299722672, 32.821144104, 30.215837479,
                                      30.543056488 };
   std::valarray<double> out_data = { 40.738149536, 42.944196384, 36.055770459,
                                      33.992358886, 28.144277061, 33.946257060,
                                      16.282855822, 32.750394182, 30.175836727,
                                      30.500680295 };

   im.setup( 1, in_data.size() );
   im.clock_charge( in_data );

   //for (unsigned int i=0;i<in_data.size();++i)
   //     std::cout << std::fixed << std::setprecision(9) << in_data[i] << ", ";
   //std::cout << std::endl;

   BOOST_CHECK_EQUAL( check_array( in_data, out_data, 1e-5), true );
}

BOOST_AUTO_TEST_SUITE_END()
