//#define BOOST_TEST_MODULE strstr

#include <boost/test/unit_test.hpp>

#include "strstr.hh"


BOOST_AUTO_TEST_SUITE( CString )

BOOST_AUTO_TEST_CASE( strtok )
{
  std::CString test_str1 = "this,is,comma,delimited,,,"; 
  std::CString test_str2 = ",,,more,commas;";
  std::CString test_str3 = "no delimiters?";
  std::CString test_str4 = "";

  BOOST_CHECK_EQUAL( test_str1.strtok(true, ","), "this");
  // continue at previos position
  BOOST_CHECK_EQUAL( test_str1.strtok(false, ","), "is");
  BOOST_CHECK_EQUAL( test_str1.strtok(false, "a"), ",comm");
  // and back to start
  BOOST_CHECK_EQUAL( test_str1.strtok(true, ","), "this");
  // check if initial delimiters are ignored
  BOOST_CHECK_EQUAL( test_str2.strtok(true, ","), "more");
  // should start from beginning because position is initialized in string constructor
  BOOST_CHECK_EQUAL( test_str3.strtok(false, "e"), "no d");
  // mind that only the 1st character of the delimiter string is actually  being used
  // I test this just to ensure consistent behaviour
  BOOST_CHECK_EQUAL( test_str3.strtok(true, "lymyt"), "no de");
  BOOST_CHECK_EQUAL( test_str3.strtok(true, ""), "no delimiters?");
  BOOST_CHECK_EQUAL( test_str4.strtok(true, "any"), "");
}


BOOST_AUTO_TEST_CASE( strstr_clean_start_test1 )
{
  std::CString test_str1 = "    space     ";
  std::CString test_str2 = "cCorrect";
  std::CString test_str3 = "closed spaces";

  BOOST_CHECK_EQUAL( test_str1.clean_start(' '), "space     ");
  BOOST_CHECK_EQUAL( test_str2.clean_start('c'), "Correct");
  BOOST_CHECK_EQUAL( test_str3.clean_start(' '), "closed spaces");
}


BOOST_AUTO_TEST_CASE( strstr_clean_char_test1 )
{
  std::CString test_str1 = "leckere Erdbeermarmelade";
  std::CString test_str2 = "X";
  std::CString test_str3 = "";
  std::CString test_str4 = "Euclid Go!";

  BOOST_CHECK_EQUAL( test_str1.clean_char('e'), "lckr Erdbrmarmlad");  
  BOOST_CHECK_EQUAL( test_str2.clean_char('X'), "");
  BOOST_CHECK_EQUAL( test_str3.clean_char('o'), "");
  BOOST_CHECK_EQUAL( test_str4.clean_char('Z'), "Euclid Go!");
}


BOOST_AUTO_TEST_CASE( strstr_clean_white_test1 )
{
  std::CString test_str1 = " ^v^ o_o ";
  std::CString test_str2 = "     ";

  BOOST_CHECK_EQUAL( test_str1.clean_white(), "^v^o_o");
  BOOST_CHECK_EQUAL( test_str2.clean_white(), "");
}


BOOST_AUTO_TEST_CASE( strstr_toupper_test1 )
{
  std::CString test_str1 = " 12345$%*-_;:,.?";
  std::CString test_str2 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  std::CString test_str3 = "";

  BOOST_CHECK_EQUAL( test_str1.toupper(), " 12345$%*-_;:,.?");
  BOOST_CHECK_EQUAL( test_str2.toupper(), "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ");
  BOOST_CHECK_EQUAL( test_str3.toupper(), "");
}

BOOST_AUTO_TEST_SUITE_END()
