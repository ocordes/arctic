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
   changed by: Oliver Cordes 2017-05-24

   $Id$

*/

#include <cstdlib>

#include <iostream>

#include "output.hh"
#include "params.hh"
#include "params_fits.hh"
#include "strstr.hh"


#define min_limit( x ) if ( ( x ) < 0 ) x = 0


// helper output function
void output_range( std::valarray<double> & x )
{
  for (unsigned int n=0;n<x.size();++n)
  {
    output( 10, "   %f\n", x[n] );
  }
}


params_fits::params_fits() : params()
{
  static_trap_definitions = 0;   //  use informations from FITS header

  working_mode            = "FITS";
}


void params_fits::parse_args_image( std::string & key, std::string & val, int & error )
{
  // output( 1, "key = %s, val = %s\n", key.c_str(), val.c_str() );
  if ( key == "XRANGE" )
  {
    str2minmax_long( val, start_x, end_x );
    output( 10, "params: xrange=%i,%i\n", start_x, end_x );

    // correct an check values
    --start_x;
    min_limit( start_x );

    error = PARSE_OK;
  }

  if ( key == "YRANGE" )
  {
    str2minmax_long( val, start_y, end_y );
    output( 10, "params: yrange=%i,%i\n", start_y, end_y );

    // correct an check values
    --start_y;
    min_limit( start_y );

    error = PARSE_OK;
  }

  if ( key == "READOUT_OFFSET" )
  {
    readout_offset = atoi( val.c_str() );
    output( 10, "params: readout_offset=%i\n", readout_offset );
    error = PARSE_OK;
  }
}

void params_fits::parse_args_cte( std::string & key, std::string & val, int & error )
{
  if ( key == "WELL_DEPTH" )
  {
    well_depth = atof( val.c_str() );
    output( 10, "params: well_depth=%f\n", well_depth );
    error = PARSE_OK;
  }

  if ( key == "WELL_NOTCH_DEPTH" )
  {
    well_notch_depth = atof( val.c_str() );
    output( 10, "params: well_notch_depth=%e\n", well_notch_depth );
    error = PARSE_OK;;
  }

  if ( key == "WELL_FILL_POWER" )
  {
    well_fill_power = atof( val.c_str() );
    output( 10, "params: well_fill_power=%f\n", well_fill_power );
    error = PARSE_OK;;
  }

  if ( key == "EMPTY_TRAP_LIMIT" )
  {
    empty_trap_limit = atof( val.c_str() );
    output( 10, "params: empty_trap_limit=%f\n", empty_trap_limit );
    error = PARSE_OK;
  }

  if ( key == "CHECK_EMPTY_TRAPS" )
  {
    bool b = tobool( val,  true );

    check_empty_traps = b;

    output( 10, "params: check_empty_traps=%i\n", check_empty_traps );
    error = PARSE_OK;
  }

}


void params_fits::parse_args_algorithm_basics( std::string & key, std::string & val, int & error )
{
  if ( key == "N_ITERATIONS" )
  {
    n_iterations = atoi( val.c_str() );
    output( 10, "params: n_iterations=%i\n", n_iterations );
    error = PARSE_OK;
  }

  if ( key == "EXPRESS" )
  {
    express = atoi( val.c_str() );
    output( 10, "params: express=%i\n", express );
    error = PARSE_OK;
  }

  if ( key == "INJECTION" )
  {
    charge_injection = true;
    output( 10, "params: charge_injection=%i\n", charge_injection );
    error = PARSE_OK;
  }

  if ( key == "CLOCK" )
  {
    unclock = false;
    output( 10, "params: clock=true -> unclock=%i\n", unclock );
    error = PARSE_OK;
  }

  if ( key == "UNCLOCK" )
  {
    unclock = tobool( val, true );
    output( 10, "params: unclock=%i\n", unclock );
    error = PARSE_OK;
  }

  if ( key == "CUT_UPPER_LIMIT" )
  {
    cut_upper_limit = tobool( val, true );
    output( 10, "params: cut_upper_limit=%i\n", cut_upper_limit );
    error = PARSE_OK;
  }

  if ( ( key == "UPPER_LIMIT" ) || ( key == "SATLEVEL" ) )
  {
    upper_limit = atof( val.c_str() );
    cut_upper_limit = true;
    output( 10, "params: upper_limit=%f\n", upper_limit );
    error = PARSE_OK;
  }
}


void  params_fits::parse_args_algorithm( std::string & key, std::string & val, int & error )
{
  if ( key == "CLASSIC" )
  {
    bool b = tobool( val, true );

    if ( b )
    {
      algorithm = ALGORITHM_CLASSIC;
    }
    else
    {
      algorithm = ALGORITHM_NEO;
    }

    output( 10, "params: ALGORITHM = %s\n", algorithm_names[algorithm].c_str() );
    error = PARSE_OK;
    return;
  }

  if ( key == "NEO" )
  {
    bool b = tobool( val, true );

    if ( b )
    {
      algorithm = ALGORITHM_NEO;
    }
    else
    {
      algorithm = ALGORITHM_CLASSIC;
    }

    output( 10, "params: ALGORITHM = %s\n", algorithm_names[algorithm].c_str() );
    error = PARSE_OK;
    return;
  }

  if ( key == "NEO2" )
  {
    bool b = tobool( val, true );

    if ( b )
    {
      algorithm = ALGORITHM_NEO2;
    }
    else
    {
      algorithm = ALGORITHM_CLASSIC;
    }

    output( 10, "params: ALGORITHM = %s\n", algorithm_names[algorithm].c_str() );

    error = PARSE_OK;
    return;
  }

  if ( key == "DARK_MODE" )
  {
    dark_mode = tobool( val, true );
    output( 10, "params: dark_mode=%i\n", dark_mode );
    error = PARSE_OK;
  }

  if ( key == "NEO2_SPLIT_LIMIT" )
  {
    neo2_split_limit = atof( val.c_str() );
    output( 10, "params: neo2_split_limit=%f\n", neo2_split_limit );
    error = PARSE_OK;
  }
}


void params_fits::parse_args_traps( std::string & key, std::string & val, int & error )
{
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
    output_range( trap_density );
    error = PARSE_OK;
    return;
  }

  if ( key == "TRAP_LIFETIME" )
  {
    static_trap_definitions = 1;           // overwrite FITS informations
    trap_lifetime = str2array( val );
    output( 10, "params: trap_lifetime=\n" );
    output_range( trap_lifetime );
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
}


void params_fits::parse_args_imagedir( std::string & key, std::string &, int & error )
{
  if ( key == "ROTATE" )
  {
    rotate = ~rotate;
    if ( rotate == image_readout_y )
    {
      output( 10, "params: readout_y selected\n" );
    }
    else
    {
      output( 10, "params: readout_x selected\n" );
    }
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


void params_fits::parse_args( std::string & key, std::string & val, int & error )
{
  output( 11, "params_fits::parse_args( key=%s, val=%s)\n", key.c_str(), val.c_str() );

  parse_args_image( key, val, error );
  parse_args_cte( key, val, error );
  parse_args_algorithm_basics( key, val, error );
  parse_args_algorithm( key, val, error );
  parse_args_traps( key, val, error );
  parse_args_imagedir( key, val, error );
}


void params_fits::check_params ( void )
{
}


void syntax_fits( void )
{
  std::cout << "    XRANGE            : specify colums to work on, comma seperated, <start>,<end>"  << std::endl;
  std::cout << "    YRANGE            : specify rows to work on, comma seperated, <start>,<end>" << std::endl;
  std::cout << "    WELL_DEPTH        : the CCD well depth"  << std::endl;
  std::cout << "    WELL_NOTCH_DEPTH  : the CCD well_notch_depth"  << std::endl;
  std::cout << "    WELL_FILL_POWER   : the CCD well_fill_power" << std::endl;
  std::cout << "    EMPTY_TRAP_LIMIT  : specify the value to mark a trap empty" << std::endl;
  std::cout << "    CHECK_EMPTY_TRAPS : check for empty traps (default) for the classic mode" << std::endl;
  std::cout << "    N_ITERATIONS      : nr of iterations for the CTE correction"  << std::endl;
  std::cout << "    EXPRESS           : express value" << std::endl;
  std::cout << "    N_LEVELS          : specify the trap sub splitting for the classic and neo mode" << std::endl;
  std::cout << "    CLOCK             : clock the image add the CTE trail" << std::endl;
  std::cout << "    UNCLOCK           : unclock the image sub the CTE trail (default)" << std::endl;
  std::cout << "    DARK_MODE         : add some optimizations for the neo and neo2 mode" << std::endl;
  std::cout << "    READOUT_OFFSET    : skip this amount of pixels in correction direction" << std::endl;
  std::cout << "    N_SPECIES         : nr of traps species"  << std::endl;
  std::cout << "    TRAP_DENSITY      : specify the trap density for each specie, comma seperated" << std::endl;
  std::cout << "    TRAP_LIFETIME     : specify the trap lifetime for each specie, comma seperated" << std::endl;
  std::cout << "    CLASSIC           : choose the classic algorithm" << std::endl;
  std::cout << "    NEO               : choose the NEO algorithm (default)" << std::endl;
  std::cout << "    NEO2              : choose the NEO2 algorithm (EXPERIMENTAL)" << std::endl;
  std::cout << "    CUT_UPPER_LIMIT   : bool , cut the upper pixel value, use with upper_limit"  << std::endl;
  std::cout << "    UPPER_LIMIT       : cut the upper pixel value to this parameter" << std::endl;
  std::cout << "    NEO2_SPLIT_LIMIT  : split limit for the neo2 algorithm"  << std::endl;
  std::cout << "    ROTATE            : change from x to y or from y to xworking mode " << std::endl;
  std::cout << "    READOUT_X         ," << std::endl;
  std::cout << "    SERIAL            : working in seriall mode" << std::endl;
  std::cout << "    READOUT_Y         ,"  << std::endl;
  std::cout << "    PARALLEL          : working in parallel mode (default)" << std::endl;
  std::cout << "    FORWARD           : working in forward mode (default)" << std::endl;
  std::cout << "    REVERSE           : working in reverse mode" << std::endl;
}
