/* (C) Copyright 2010 by Oliver Cordes
        - ocordes ( at ) astro ( dot ) uni-bonn ( dot ) de


    This file is part of arctic.

    arctic is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    arctic is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with arctic.  If not, see <http://www.gnu.org/licenses/>.

*/

/* output.cc

   written by: Oliver Cordes 2010-07-20
   changed by: Oliver Cordes 2017-06-21

   $Id$

*/


#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <cstdarg>
#include <time.h>

// #include <string>
#include <iostream>

#include "output.hh"


/* global variables */

int debug_level = 1;  /* be quiet */


char *debug_generate_timestamp( void )
{
  struct tm   *timep;
  time_t       times;
  char        *p;

  times = time( NULL );
  timep = localtime( &times );

  if ( asprintf( &p, "%02i:%02i.%02i", timep->tm_hour, timep->tm_min, timep->tm_sec ) == -1 )
  {
    p = strdup( "asprintf error state" );
  }

  return p;
}


// special function to parse the debug level

void debug_init( int argc, char *argv[] )
{
  for (int i=1;i<argc;i++)
  {
    if ( std::strcmp( argv[i], "-d" ) == 0 )
    {
      i++;
      if ( i == argc )
      {
        std::cout << "Warning: no debug parameter given! Returning default values!" << std::endl;
        return;
      }
      else
      {
        debug_level = atoi( argv[i] );
        //std::cout << "new debug level=" << debug_level << std::endl;
      }
    }
  }
}


bool is_debug( int dlevel )
{
  // be completly quiet or do nothing
  if ( debug_level == 0 )
  {
    return false;
  }

  return dlevel <= debug_level;
}


void output( int dlevel, const char *format, ... )
{
  char *os;

  if ( is_debug( dlevel) )
  {
    va_list ap;

    // generate output
    va_start( ap, format );
    int erg = vasprintf( &os, format, ap );
    va_end( ap );

    // check a possible error state
    if ( erg == -1 )
    {
      os = strdup( "vasprintf error state" );
    }

    if ( debug_level == 1 )
    {
      printf( "%s", os );
    }
    else
    {
      char *ts = debug_generate_timestamp();
      printf( "%s: %s", ts, os );
      free( ts );
    }

    free( os );

    fflush( stdout );
  }
}
