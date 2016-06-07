/* (C) Copyright 2013 by Oliver Cordes         
        - ocordes ( at ) astro ( dot ) uni-bonn ( dot ) de


    This file is part of acs-cte-project.

    acs-cte is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    acs-cte is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with acs-cte.  If not, see <http://www.gnu.org/licenses/>. 

*/

/* params_fits.cc

   written by: Oliver Cordes 2015-06-18
   changed by: Oliver Cordes 2016-03-08

   $Id: params_fits.cc 975 2016-03-09 13:08:56Z ocordes $

*/

#include <cstdlib>

#include "output.hh"
#include "params.hh"
#include "params_fits.hh"
#include "strstr.hh"


params_fits::params_fits() : params()
{
  static_trap_definitions = 0;   //  use informations from FITS header
}

void params_fits::parse_args( std::string key, std::string val )
{
  output( 11, "params_fits::parse_args( key=%s, val=%s)\n", key.c_str(), val.c_str() );

  if ( key == "XRANGE" )
    {
      xrange = str2array_long( val );
      output( 10, "params: xrange=\n" );
      for (unsigned int n=0;n<xrange.size();n++) output( 10, "   %i\n", xrange[n] );

      // correct an check values
      xrange[0]--;

      if ( xrange[0] < 0 )
        xrange[0] = 0;
      return;
    }
  if ( key == "YRANGE" )
    {
      yrange = str2array_long( val );
      output( 10, "params: yrange=\n" );
      for (unsigned int n=0;n<yrange.size();n++) output( 10, "   %i\n", yrange[n] );

      // correct an check values
      yrange[0]--;

      if ( yrange[0] < 0 )
        yrange[0] = 0;
      return;
    }
  if ( key == "WELL_DEPTH" )
    {
      well_depth = atof( val.c_str() );
      output( 10, "params: well_depth=%f\n", well_depth );
      return;
    }
  if ( key == "WELL_NOTCH_DEPTH" )
    {
      well_notch_depth = atof( val.c_str() );
      output( 10, "params: well_notch_depth=%e\n", well_notch_depth );
      return;
    }
  if ( key == "WELL_FILL_POWER" )
    {
      well_fill_power = atof( val.c_str() );
      output( 10, "params: well_fill_power=%f\n", well_fill_power );
      return;
    }
  if ( key == "EMPTY_TRAP_LIMIT" )
    {
      empty_trap_limit = atof( val.c_str() );
      output( 10, "params: empty_trap_limit=%f\n", empty_trap_limit );
      return;
    }
  if ( key == "CHECK_EMPTY_TRAPS" )
    {
      bool b = true;
      
      if ( val != "" )
	b = tobool( val );
      check_empty_traps = b;

      output( 10, "params: check_empty_traps=%i\n", check_empty_traps );
      return;
    }

  
  if ( key == "N_ITERATIONS" )
    {
      n_iterations = atoi( val.c_str() );
      output( 10, "params: n_iterations=%i\n", n_iterations );
      return;
    }
  if ( key == "EXPRESS" )
    {
      express = atoi( val.c_str() );
      output( 10, "params: express=%i\n", express );
      return;
    }
  if ( key == "N_LEVELS" )
    {
      n_levels = atoi( val.c_str() );
      output( 10, "params: n_levels=%i\n", n_levels );
      return;
    }
  if ( key == "UNCLOCK" )
    {
      bool b = true;

      if ( val != "" )
	b = tobool( val );
      
      unclock = b;
      output( 10, "params: unclock=%i\n", unclock );
      return;
    }
  if ( key == "DARK_MODE" )
    {
      bool b = true;

      if ( val != "" )
	b = tobool( val );
      
      dark_mode = b;
      output( 10, "params: dark_mode=%i\n", dark_mode );
      return;
    }
  if ( key == "READOUT_OFFSET" )
    {
      readout_offset = atoi( val.c_str() );
      output( 10, "params: readout_offset=%i\n", readout_offset );
      return;
    }
  if ( key == "N_SPECIES" )
    {
      n_species = atoi( val.c_str() );
      output( 10, "params: n_species=%i\n", n_species );
      return;
    }
  if ( key == "TRAP_DENSITY" )
    {
      static_trap_definitions = 1;           // overwrite FITS informations
      trap_density = str2array( val );
      output( 10, "params: trap_density=\n" );
      for (unsigned int n=0;n<trap_density.size();n++) output( 10, "   %f\n", trap_density[n] );
      return;
    }
  if ( key == "TRAP_LIFETIME" )
    {
      static_trap_definitions = 1;           // overwrite FITS informations      
      trap_lifetime = str2array( val );
      output( 10, "params: trap_lifetime=\n" );
      for (unsigned int n=0;n<trap_lifetime.size();n++) output( 10, "   %f\n", trap_lifetime[n] );
      return;
    }
  if ( key == "CLASSIC" )
    {
      bool b = true;

      if ( val != "" )
	b = tobool( val );

      if ( b )
	{
	  neo_algorithm = false;
	  neo_algorithm2 = false;
	}
      else
	{
	  neo_algorithm = true;
	  neo_algorithm2 = false;
	}
      
      output( 10, "params: classic_algorithm=%i\n", b );
      output( 10, "params: neo_algorithm=%i\n", neo_algorithm );
      return;
    }
  if ( key == "NEO" )
    {
      bool b = true;

      if ( val != "" )
	b = tobool( val );
      
      neo_algorithm = b;

      if ( b )
	  neo_algorithm2 = false;
      
      output( 10, "params: neo_algorithm=%i\n", neo_algorithm );
      return;
    }
  if ( key == "NEO2" )
    {
       bool b = true;

      if ( val != "" )
	b = tobool( val );
      
      neo_algorithm2 = b;

       if ( b )
	  neo_algorithm = false;
      
      output( 10, "params: neo_algorithm2=%i\n", neo_algorithm2 );
      return;
    }
  if ( key == "CUT_UPPER_LIMIT" )
    {
       bool b = true;

      if ( val != "" )
	b = tobool( val );
      
      cut_upper_limit = b;
      output( 10, "params: cut_upper_limit=%i\n", cut_upper_limit );
      return;
    }

  if ( key == "UPPER_LIMIT" )
    {
      upper_limit = atof( val.c_str() );
      output( 10, "params: upper_limit=%f\n", upper_limit );
      return;
    }
  
}
