//#define BOOST_TEST_MODULE strstr

#include <boost/test/unit_test.hpp>

#include <boost/test/output_test_stream.hpp>
#include <iostream>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <cstdarg>
#include <time.h>

#include "output.hh"

// written by: Oliver Cordes 2017-05-31
// changed by: Oliver Cordes 2017-07-12


BOOST_AUTO_TEST_SUITE( output_test_suite )

struct cout_redirect {
    cout_redirect( std::streambuf * new_buffer )
        : old( std::cout.rdbuf( new_buffer ) )
    { }

    ~cout_redirect( ) {
        std::cout.rdbuf( old );
    }

private:
    std::streambuf * old;
};

std::string timestamp( time_t seconds, char *msg )
{
  struct tm   *timep;
  char        *p;

  timep = localtime( &seconds );

  if ( asprintf( &p, "%02i:%02i.%02i: %s", timep->tm_hour, timep->tm_min, timep->tm_sec, msg ) == -1 )
  {
    p = strdup( "asprintf error state" );
  }

  std::string s( p );

  free( p );

  return s;

}


// test parsing debug arguments
BOOST_AUTO_TEST_CASE( debug_init_test )
{
  char **argv_test;
  argv_test = (char**) malloc( sizeof( void * ) * 3);
  argv_test[0] = strdup( "Program_name_test" );
  argv_test[1] = strdup( "-d" );
  argv_test[2] = strdup( "100" );

  debug_level = -1;

  debug_init( 3, argv_test );

  BOOST_CHECK_EQUAL( debug_level, 100 );

  free( argv_test[0] );
  free( argv_test[1] );
  free( argv_test[2] );
  free( argv_test );
}


// test is_debug functions
BOOST_AUTO_TEST_CASE( is_debug_test )
{
  debug_level = 100;

  BOOST_CHECK_EQUAL( is_debug( 0 ), true );
  BOOST_CHECK_EQUAL( is_debug( 100 ), true );
  BOOST_CHECK_EQUAL( is_debug( 101 ), false );

  debug_level = 0;  /* be quiet at all */

  BOOST_CHECK_EQUAL( is_debug( 0 ), false );
  BOOST_CHECK_EQUAL( is_debug( 100 ), false );
}


BOOST_AUTO_TEST_CASE( output_test )
{
  time_t seconds;
  bool   check;

  boost::test_tools::output_test_stream soutput;
  {
      cout_redirect guard( soutput.rdbuf( ) );

      debug_level = 100;
      output( 10, "Test\n" );
  }

  seconds = time( NULL );
  std::string s1 = timestamp( seconds, (char*) "Test\n" );
  std::string s2 = timestamp( seconds-1, (char*) "Test\n" );
  check = false;
  check = soutput.is_equal( s1, false );
  if ( check == false)
  {
    check = soutput.is_equal( s2, false );
  }
  BOOST_CHECK( check );
}

BOOST_AUTO_TEST_SUITE_END()
