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

/* params_fits.cc

   written by: Oliver Cordes 2015-06-18
   changed by: Oliver Cordes 2016-06-29

   $Id$

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

void params_fits::parse_args( std::string key, std::string val, int & error )
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

      error = PARSE_OK;
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

      error = PARSE_OK;
      return;
    }
  if ( key == "WELL_DEPTH" )
    {
      well_depth = atof( val.c_str() );
      output( 10, "params: well_depth=%f\n", well_depth );
      error = PARSE_OK;
      return;
    }
  if ( key == "WELL_NOTCH_DEPTH" )
    {
      well_notch_depth = atof( val.c_str() );
      output( 10, "params: well_notch_depth=%e\n", well_notch_depth );
      error = PARSE_OK;
      return;
    }
  if ( key == "WELL_FILL_POWER" )
    {
      well_fill_power = atof( val.c_str() );
      output( 10, "params: well_fill_power=%f\n", well_fill_power );
      error = PARSE_OK;
      return;
    }
  if ( key == "EMPTY_TRAP_LIMIT" )
    {
      empty_trap_limit = atof( val.c_str() );
      output( 10, "params: empty_trap_limit=%f\n", empty_trap_limit );
      error = PARSE_OK;
      return;
    }
  if ( key == "CHECK_EMPTY_TRAPS" )
    {
      bool b = true;

      if ( val != "" )
	       b = tobool( val );
      check_empty_traps = b;

      output( 10, "params: check_empty_traps=%i\n", check_empty_traps );
      error = PARSE_OK;
      return;
    }


  if ( key == "N_ITERATIONS" )
    {
      n_iterations = atoi( val.c_str() );
      output( 10, "params: n_iterations=%i\n", n_iterations );
      error = PARSE_OK;
      return;
    }
  if ( key == "EXPRESS" )
    {
      express = atoi( val.c_str() );
      output( 10, "params: express=%i\n", express );
      error = PARSE_OK;
      return;
    }
  if ( key == "N_LEVELS" )
    {
      n_levels = atoi( val.c_str() );
      output( 10, "params: n_levels=%i\n", n_levels );
      error = PARSE_OK;
      return;
    }
  if ( key == "CLOCK" )
  {
    unclock = false;
    output( 10, "params: clock=true -> unclock=%i\n", unclock );
    error = PARSE_OK;
    return;
  }
  if ( key == "UNCLOCK" )
    {
      bool b = true;

      if ( val != "" )
	     b = tobool( val );

      unclock = b;
      output( 10, "params: unclock=%i\n", unclock );
      error = PARSE_OK;
      return;
    }
  if ( key == "DARK_MODE" )
    {
      bool b = true;

      if ( val != "" )
	       b = tobool( val );

      dark_mode = b;
      output( 10, "params: dark_mode=%i\n", dark_mode );
      error = PARSE_OK;
      return;
    }
  if ( key == "READOUT_OFFSET" )
    {
      readout_offset = atoi( val.c_str() );
      output( 10, "params: readout_offset=%i\n", readout_offset );
      error = PARSE_OK;
      return;
    }
  if ( key == "N_SPECIES" )
    {
      n_species = atoi( val.c_str() );
      output( 10, "params: n_species=%i\n", n_species );
      error = PARSE_OK;
      return;
    }
  if ( key == "TRAP_DENSITY" )
    {
      static_trap_definitions = 1;           // overwrite FITS informations
      trap_density = str2array( val );
      output( 10, "params: trap_density=\n" );
      for (unsigned int n=0;n<trap_density.size();n++) output( 10, "   %f\n", trap_density[n] );
      error = PARSE_OK;
      return;
    }
  if ( key == "TRAP_LIFETIME" )
    {
      static_trap_definitions = 1;           // overwrite FITS informations
      trap_lifetime = str2array( val );
      output( 10, "params: trap_lifetime=\n" );
      for (unsigned int n=0;n<trap_lifetime.size();n++) output( 10, "   %f\n", trap_lifetime[n] );
      error = PARSE_OK;
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
      error = PARSE_OK;
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
      error = PARSE_OK;
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
      error = PARSE_OK;
      return;
    }
  if ( key == "CUT_UPPER_LIMIT" )
    {
       bool b = true;

      if ( val != "" )
	b = tobool( val );

      cut_upper_limit = b;
      output( 10, "params: cut_upper_limit=%i\n", cut_upper_limit );
      error = PARSE_OK;
      return;
    }

  if ( key == "UPPER_LIMIT" )
    {
      upper_limit = atof( val.c_str() );
      output( 10, "params: upper_limit=%f\n", upper_limit );
      error = PARSE_OK;
      return;
    }
  if ( key == "ROTATE" )
  {
    rotate = ~rotate;
    if ( rotate == image_readout_y )
      output( 10, "params: readout_y selected\n" );
    else
      output( 10, "params: readout_x selected\n" );
    error = PARSE_OK;
    return;
  }
  if ( ( key == "READOUT_X" ) || ( key == "SERIAL") )
  {
    rotate = image_readout_x;
    output( 10, "params: readout_x selected\n" );
    error = PARSE_OK;
    return;
  }
  if ( ( key == "READOUT_Y" ) || ( key == "PARALLEL") )
  {
    rotate = image_readout_y;
    output( 10, "params: readout_y selected\n" );
    error = PARSE_OK;
    return;
  }
  if ( key == "FORWARD" )
  {
    direction = image_forward;
    output( 10, "params: readout_forward selected\n" );
    error = PARSE_OK;
    return;
  }
  if ( key == "REVERSE" )
  {
    direction = image_reverse;
    output( 10, "params: readout_reverse selected\n" );
    error = PARSE_OK;
    return;
  }

}

void params_fits::check_params ( void )
{
}
