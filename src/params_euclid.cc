/* (C) Copyright 2013 by Oliver Cordes
        - ocordes ( at ) astro ( dot ) uni-bonn ( dot ) de


    This file is part of arctic-project.

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

/* params_euclid.cc

   written by: Oliver Cordes 2015-06-18
   changed by: Oliver Cordes 2017-05-31

   $Id$

*/

#include <cstdlib>
#include <iostream>

#include "output.hh"
#include "params_fits.hh"
#include "params_euclid.hh"
#include "strstr.hh"


params_euclid::params_euclid() : params_fits(),
  has_local_n_species( false ),      // try to load the mission database
  has_local_trap_density( false ),   // try to load the mission database
  ccd( 0 )
{
  working_mode = "EUCLID";
}


void params_euclid::set_args( int *argc, char **argv[]  )
{
  output( 1, "params:euclid::set_args called\n" );

  params_fits::set_args( argc, argv );

  // put in here the code for reading the euclid mission database

  // load_global_mission_database();
  // load_local_mission_database();

  // copy only data from the database if necessary
  if ( has_local_n_species == true)
  {
    output( 10, "Using local n_species ...\n" );
  }
  else
  {
    // copy data from mission database
  }
  if ( has_local_trap_density == true)
  {
    output( 10, "Using local trap_density ...\n" );
  }
  else
  {
    // copy data from mission database
  }
}

void params_euclid::parse_args( std::string & key, std::string & val, int & error )
{
  output( 11, "params_euclid::parse_args( key=%s, val=%s)\n", key.c_str(), val.c_str() );
  params_fits::parse_args( key, val, error );

  if ( key == "CCD" )
    {
      ccd = atoi( val.c_str() );
      output( 10, "params: ccd=%i\n", ccd );
      error = PARSE_OK;
      return;
    }


  if ( key == "N_SPECIES" )
  {
    has_local_n_species = true;
    return;
  }

  if ( key == "TRAP_DENSITY" )
  {
    has_local_trap_density = true;
    return;
  }
}


void params_euclid::check_params ( void )
{
}


void syntax_euclid( void )
{
  std::cout << "    none" << std::endl;
}
