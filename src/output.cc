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

/* output.c

   written by: Oliver Cordes 2010-07-20
   changed by: Oliver Cordes 2015-11-30

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


char dummy[1000];


char timestamp[20];


void debug_generate_timestamp( void )
{
  struct tm   *timep;
  time_t       times;

  times = time( NULL );
  timep = localtime( &times );

  snprintf( timestamp, 20, "%02i:%02i.%02i",
	          timep->tm_hour, timep->tm_min, timep->tm_sec );
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




void output( int dlevel, const char *format, ... )
{
  /* be completly quiet */
  if ( debug_level == 0 )
    return;

  if ( dlevel <= debug_level )
  {
    va_list ap;

	  /* generate output */
	  va_start( ap, format );
	  vsnprintf( dummy, 1000, format, ap );
	  va_end( ap );

	  if ( debug_level == 1 )
	    printf( "%s", dummy );
	  else
	  {
	    debug_generate_timestamp();
	    printf( "%s: %s", timestamp, dummy );
	  }

	  fflush( stdout );
  }
}
