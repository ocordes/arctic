/* (C) Copyright 2013 by Oliver Cordes
        - ocordes ( at ) astro ( dot ) uni-bonn ( dot ) de


    This file is part of arctic project.

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

/* image_acs.cc

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2017-05-11

   $Id$

*/

//#define _GLIBCXX_USE_CXX11_ABI 0

#include <cstdlib>
#include <cstring>
#include <ctime>

#include <iostream>

#include <CCfits/CCfits>

#include "config.h"
#include "cte_image.hh"
#include "cte_image_classic.hh"
#include "cte_image_neo.hh"
#include "cte_image_watermark.hh"
#include "image.hh"
#include "image_acs.hh"
#include "julian.hh"
#include "output.hh"
#include "params_acs.hh"


using namespace CCfits;


acs_image::acs_image( int argc, char *argv[] ) : image( argc, argv )
{
  // alloctae parameter object
  parameters  = std::shared_ptr<params_acs>( new params_acs() );
  // initialize with command line parameters
  parameters->set_args( &argc, &argv);

  if ( argc < 2 )
  {
    output( 1, "Program needs <INPUTFILE> <OUTPUTFILE>! Abort\n" );
    throw "Not enough arguments!";
  }

  // copy filename parameters
  infilename = argv[1];
  outfilename = argv[2];

  sci_mode_dark = false;

  electrons_per_sec = false;
}


acs_image::~acs_image()
{
}


int acs_image::clock_charge_prepare( void )
{
  // some preparations

  // type cast to the correct param object definition
  // syntax is horrible
  std::shared_ptr<params_acs> params = std::dynamic_pointer_cast <params_acs> ( parameters);

  // read/analyze keywords


  // first check the date keyword
  double date = readkey<double>( FITS_image->pHDU(), "DATE" );

  if ( std::isnan( date ) )
  {
    std::string sdate = readkey<std::string>( FITS_image->pHDU(), "DATE" );
    date = date2double( sdate );
  }
  else
  {
    output( 1, "Image aquired after %f days since launch\n", date );

    date += +2452334.5; /* convert to JD */
  }


  int erg = correct_units();

  if ( erg == 1 )
  {
    // if something went wrong in unit check, then exit the program!
    return 1;
  }

  // calculate the trap confiuration

  params->calc_trap_config( date );


  return 0;
}



// internal private functions

double  acs_image::date2double( std::string & s )
{
  struct tm stime;

  if ( strptime( s.c_str(), "%Y-%m-%d", &stime ) != NULL )
    {
      stime.tm_hour = 0;
      stime.tm_min = 0;
      stime.tm_sec = 0;
      output( 10, "strptime: y=%i m=%i d=%i H=%i M=%i S=%i\n",
              stime.tm_year+1900,
              stime.tm_mon+1,
              stime.tm_mday,
              stime.tm_hour,
              stime.tm_min,
              stime.tm_sec );
      return juliandate( stime.tm_year+1900,
                         stime.tm_mon+1,
                         stime.tm_mday,
                         stime.tm_hour,
                         stime.tm_min,
			 stime.tm_sec );
    }

  output( 1, "strptime failed for '%s'!\n", s.c_str() );
  return 0.0;
}
