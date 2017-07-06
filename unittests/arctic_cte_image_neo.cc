//#define BOOST_TEST_MODULE strstr

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <iomanip>

#include "cte_image_neo.hh"
#include "params.hh"
#include "params_acs.hh"
#include "output.hh"

// written by: Oliver Cordes 2017-06-01
// changed by: Oliver Cordes 2017-07-03


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


BOOST_AUTO_TEST_SUITE( cte_mage_neo_test_suite )

BOOST_AUTO_TEST_CASE( constructor )
{
  cte_image_neo im;

  //BOOST_CHECK_EQUAL( im.prgname, "Program_name_test" );
}

// check the following parameters
//  - unlock
//  - no rotation
//  - express = 1
//  - n_iteration = 1
//  - cut_upper_limits = false
//  - high signal to noise
BOOST_AUTO_TEST_CASE( correction1 )
{
   std::shared_ptr<params_acs> p = std::shared_ptr<params_acs>( new params_acs() );

   double jd = +2452334.5 + 3506.238668981474;

   p->calc_trap_config( jd );

   cte_image_neo im( (std::shared_ptr<params> &) p );

   std::valarray<double> in_data =  { 40.787811279, 42.996334076, 36.101192474,
                                      34.036590576, 28.182380676, 33.998012543,
                                      16.299722672, 32.821144104, 30.215837479,
                                      30.543056488 };
   //std::valarray<double> out_data = { 40.837474823, 43.048473358, 36.146598816,
   //                                   34.080699921, 28.220380783, 34.049839020,
   //                                   16.316484451, 32.891876221, 30.255695343,
   //                                   30.585535049 };
   std::valarray<double> out_data = { 40.837474823, 43.048473358, 36.146598816,
                                      34.080699921, 28.220380783, 34.049839020,
                                      16.316484451, 32.891876221, 30.255695343,
                                      30.585535049 };


   im.setup( 1, in_data.size() );
   im.clock_charge( in_data );

   BOOST_CHECK_EQUAL( check_array( in_data, out_data, 1e-5), true );
}

// check the following parameters
//  - unlock
//  - no rotation
//  - express = 1
//  - n_iteration = 1
//  - cut_upper_limits = false
//  - low signal to noise
BOOST_AUTO_TEST_CASE( correction2 )
{
   std::shared_ptr<params_acs> p = std::shared_ptr<params_acs>( new params_acs() );

   //double jd = +2452334.5 + 3506.238668981474;
   double jd = +2452334.5 + 2786.02;

   p->calc_trap_config( jd );

   cte_image_neo im( (std::shared_ptr<params> &) p );

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

   std::valarray<double> out_data = { 0.009424694, 0.003115607, 0.001802142, 0.001963123,
                                      0.001051654, 0.001478778, 0.001716571, 0.001196603,
                                      -0.000469148, 0.005556351, 0.024765190, 0.015526458,
                                      0.001667337, 0.001063919, 0.000727044, 0.009214219,
                                      0.005197967, 0.346392241, -0.005171691, 0.018469184,
                                      -0.001660591, 0.000416505, -0.001410909, -0.001998813,
                                      -0.002294084, 0.002223993, -0.000516971, 0.040910933,
                                      0.004549861, 0.001265894, 0.007010214, 0.002640427,
                                      -0.000731729, -0.002146611, -0.002358554, 0.002252047,
                                      0.002123638, 0.012249101, -0.001816263, 0.001202978,
                                      -0.000300440, 0.011447032, -0.000700869, 0.020856006,
                                      0.000491963, 0.017139437, 0.019917083, 0.001435274,
                                      -0.001097397, 0.002108015, 0.001904715, 0.001896253,
                                      0.000602449, -0.001489135, 0.003961337, 0.001623183,
                                      0.004253726, 0.007371266, 0.028278507, -0.002527334,
                                      0.000005146, 0.012300347, 0.003748590, 0.002129830,
                                      0.001444138, 0.002909304, 0.001081251, 0.000614425,
                                      0.000574376, 0.000535724, 0.005696510, 0.060418145,
                                      0.010415723, -0.002959106, 0.021258633, -0.001940383,
                                      0.001265823, 0.131244462, -0.006473487, -0.006508577 };


   im.setup( 1, in_data.size() );
   //debug_level = 10;
   im.clock_charge( in_data );

   //for (unsigned int i=0;i<in_data.size();++i)
   // std::cout << std::fixed << std::setprecision(9) << in_data[i] << ", ";
   // std::cout << std::endl;

   BOOST_CHECK_EQUAL( check_array( in_data, out_data, 1e-9), true );
}

BOOST_AUTO_TEST_SUITE_END()
