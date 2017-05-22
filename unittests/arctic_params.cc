#define BOOST_TEST_MODULE MyTest

#include <boost/test/unit_test.hpp>

#include "params.hh"


BOOST_AUTO_TEST_SUITE( params_test_suite )

BOOST_AUTO_TEST_CASE( params_test_str2array )
{
  params p;

  std::string test_str1 = "10.3,28.7253212,-0.00001";
  std::valarray<double> res_array1 {10.3, 28.7253212, -0.00001};

  std::string test_str2 = "";
  std::valarray<double> res_array2 {};

  std::string test_str3 = "3.1415";
  std::valarray<double> res_array3 {3.1415};

  std::string test_str4 = ",-00.01,";
  std::valarray<double> res_array4 {-0.01};

  std::string test_str5 = ".007";
  std::valarray<double> res_array5 {0.007};

  // we cannot evaluate std::valarray with boost directly, so project it down into a single bool
  std::valarray<bool> a1 = p.str2array(test_str1) == res_array1;
  bool is_equal1 = std::all_of(begin(a1), end(a1), [](bool b){return b;});

  std::valarray<bool> a2 = p.str2array(test_str2) == res_array2;
  bool is_equal2 = std::all_of(begin(a2), end(a2), [](bool b){return b;});

  std::valarray<bool> a3 = p.str2array(test_str3) == res_array3;
  bool is_equal3 = std::all_of(begin(a3), end(a3), [](bool b){return b;});

  std::valarray<bool> a4 = p.str2array(test_str4) == res_array4;
  bool is_equal4 = std::all_of(begin(a4), end(a4), [](bool b){return b;});

  std::valarray<bool> a5 = p.str2array(test_str5) == res_array5;
  bool is_equal5 = std::all_of(begin(a5), end(a5), [](bool b){return b;});

  BOOST_CHECK_EQUAL(is_equal1, true);
  BOOST_CHECK_EQUAL(is_equal2, true);
  BOOST_CHECK_EQUAL(is_equal3, true);
  BOOST_CHECK_EQUAL(is_equal4, true);
  BOOST_CHECK_EQUAL(is_equal5, true);

}

BOOST_AUTO_TEST_CASE( params_test_str2array_long )
{
  params p;

  std::string test_str1 = "10,25,8";
  std::valarray<long> res_array1 {10, 25, 8};

  std::string test_str2 = "";
  std::valarray<long> res_array2 {};

  std::string test_str3 = "42";
  std::valarray<long> res_array3 {42};

  std::string test_str4 = ",-01,";
  std::valarray<long> res_array4 {-1};

  // check this:
  // str2array_long is removed from the main code OC 2017-05-17
  // // we cannot evaluate std::valarray with boost directly, so project it down into a single bool
  // std::valarray<bool> a1 = p.str2array_long(test_str1) == res_array1;
  // bool is_equal1 = std::all_of(begin(a1), end(a1), [](bool b){return b;});
  //
  // std::valarray<bool> a2 = p.str2array_long(test_str2) == res_array2;
  // bool is_equal2 = std::all_of(begin(a2), end(a2), [](bool b){return b;});
  //
  // std::valarray<bool> a3 = p.str2array_long(test_str3) == res_array3;
  // bool is_equal3 = std::all_of(begin(a3), end(a3), [](bool b){return b;});
  //
  // std::valarray<bool> a4 = p.str2array_long(test_str4) == res_array4;
  // bool is_equal4 = std::all_of(begin(a4), end(a4), [](bool b){return b;});
  //
  // BOOST_CHECK_EQUAL(is_equal1, true);
  // BOOST_CHECK_EQUAL(is_equal2, true);
  // BOOST_CHECK_EQUAL(is_equal3, true);
  // BOOST_CHECK_EQUAL(is_equal4, true);

}

BOOST_AUTO_TEST_SUITE_END()
