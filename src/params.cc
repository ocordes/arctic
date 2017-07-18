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

/* params.cc

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2017-07-18
*/

#include <cstdlib>
#include <cstring>

#include <climits>

#include <string>
#include <valarray>
#include <vector>

#include <iostream>
#include <fstream>
#include <exception>

#include "config.h"

#include "image_slice.hh"
#include "output.hh"
#include "params.hh"
#include "strstr.hh"


std::string algorithm_names[] = { "CLASSIC",
                                  "NEO",
                                  "NEO2" };


params::params() :
 trap_density( { 0 } ),
 trap_lifetime( { 0 } ),
 working_mode( "" ),
 config_filename( "" )
{
  algorithm             = ALGORITHM_NEO;
  well_depth            = 84700;
  well_notch_depth      = 1e-9;
  well_fill_power       = 0.465;
  check_empty_traps     = true;
  empty_trap_limit      = 1e-11;
  empty_trap_limit_neo2 = 1e-6;
  charge_injection      = false;
  n_iterations          = 1;
  express               = 1;
  n_levels              = 10000;
  unclock               = true;
  dark_mode             = false;
  readout_offset        = 0;
  start_x               = 0;
  end_x                 = LONG_MAX;
  start_y               = 0;
  end_y                 = LONG_MAX;

  rotate                = image_readout_y;
  direction             = image_forward;

  cut_upper_limit       = false;
  upper_limit           = 65536.-3;

  neo2_split_limit      = 1e-14;

  // only for reading parameters, not used in the CTE code
  n_species             = 0;

  force                 = false;

  output( 11, "params::params()\n" );
}


params::~params()
{
  // std::cout << "params::~params()" << std::endl;
}


int params::load_config( std::string filename )
{
  output( 1, "Loading config file '%s' ...\n", filename.c_str() );

  config_filename = filename;

  std::CString  s;

  std::ifstream f( filename, std::ifstream::in );

  if ( f.is_open() )
  {
    // file is open -> read all content
    while ( f.eof() == false )
    {
      getline(f, s);

      int error;

      // ignore empty lines and comments
      if ( ( s == "" ) || ( s[0] == '#' ) )
      {
        continue;
      }

      // clean spaces
      s = s.clean_white();

      // add line into condfig_entry array for saving information in FITS header
      config_entries.push_back( s );

      //std::cout << s << std::endl;

      std::CString key = s.strtok( true, "=#" );
      std::CString val = s.strtok( false, "=#" );

      // convert key into upper cases
      key = key.toupper();

      // std::cout << "key=" << key << " val=" << val << std::endl;

      error = PARSE_ERROR;
      parse_args( key, val, error );
      parse_error_msg( error, key, val );
    }
    f.close();
  }
  else
  {
    // some  error during the opening occured
    std::cout << "Error during opening config file '"
              << filename << "'! Config file will be ignored!" << std::endl;
    return 1;
  }

  output( 1, "Done.\n" );

  return 0;
}


// use this virtual function only for declaration
// it schould never be called!!!

void params::parse_args( std::string &, std::string &, int & )
{
  throw "Not implemented!";
}


// use this function to verify/check all given parameters
// it will be called after all parameters are set, so one
// can do consistency checks or reset parameters
// e.g. the force = false parameter an be used to overwrite
// unwanted parameters

void params::check_params( void )
{
  throw "Not implemented!";
}


void params::parse_error_msg( int status, std::string & key, std::string & val )
{
  switch ( status )
  {
    case PARSE_OK:
      break;
    case PARSE_UNKNOWN:
      std::cout << "Parse error: unhandled value for parameter  " << key << " : " << val << std::endl;
      break;
    case PARSE_ERROR:
      std::cout << "Parse error: unhandled parameter: " << key << " (val=" << val << ")" << std::endl;
      break;
  }
}


void params::set_args( int *argc, char **argv[]  )
{
  int nargc = (*argc);
  int outp  = 1;

  int error;

  output( 11, "params::set_args: called\n" );

  int i = 1; // the first argument is always the program name ;-)

  //std::cout << (*argc) << std::endl;
  while ( i < nargc )
  {
    // std::cout << (*argv)[i] << std::endl;

    // check if this an option
    if ( (*argv)[i][0] == '-' )
    {
      // read at least one argument
      (*argc)--;

      if ( ( std::strcmp( (*argv)[i], "-c" ) == 0 ) ||
           ( std::strcmp( (*argv)[i], "-d" ) == 0 ) ||
           ( std::strcmp( (*argv)[i], "-m" ) == 0 ) )
      {
        // check if one more argument is available
        if ( (i+1) < nargc )
        {
          (*argc)--;
          i++;
          if ( (*argv)[i-1][1] == 'c' )
          {
            std::string s = (*argv)[i];
            load_config( s );
          }
        }
        i++;
        continue;
      }

      // default

      // split parameter  for further checks

      std::CString s( (*argv)[i] );
      s = s.clean_start( '-' );
      std::CString key = s.strtok( true, "=#\0\n" );
      std::CString val = s.strtok( false, "=#\0\n" );

      // convert key into upper cases
      key = key.toupper();

      error = PARSE_ERROR;
      parse_args( key, val, error );
      parse_error_msg( error, key, val );
    } // end of if (*argv)[0]  == '-'
    else
    {
      if ( i != outp )
      {
        (*argv)[outp] = (*argv)[i];
        (*argv)[i] = NULL;
      }
      outp++;
    }

    i++;
  }

  // finally check parameter for consistency
  check_params();
}


std::valarray<double> params::str2array( std::CString s )
{
  std::vector<double> temp_vec;

  std::CString p = s.strtok( true, "," );
  while ( p != "" )
    {
      temp_vec.push_back( atof( p.c_str() ) );
      p = s.strtok( false, "," );
    }

  return std::valarray<double> ( &temp_vec[0], temp_vec.size() );
}


void params::str2minmax_long( std::CString s, long & min, long & max )
{
  min = 0;
  max = 0;

  std::CString p = s.strtok( true, "," );
  if ( p != "" )
  {
    min = atoi( p.c_str() );
  }
  p = s.strtok( false, "," );
  if ( p != "" )
  {
    max = atoi( p.c_str() );
  }
}


double params::val2double( std::string & s, int & error )
{
  if ( s == "" )
  {
    error = PARSE_UNKNOWN;
    return 0.0;
  }

  error = PARSE_OK;
  return atof( s.c_str() );
}


int    params::val2int( std::string & s, int & error )
{
  if ( s == "" )
  {
    error = PARSE_UNKNOWN;
    return 0;
  }

  error = PARSE_OK;
  return atoi( s.c_str() );
}

// helper functions

bool params::tobool( std::string & val, bool defaults, int & error )
{
  int b = false;

  if ( val == "" )
  {
    error = PARSE_UNKNOWN;
    return defaults;
  }

  std::CString s = val; // savety copying
  s = s.toupper();
  error = PARSE_OK;

  switch ( s[0] )
  {
    case 'Y':
    case 'J':
    case 'T':
    case '1':
      b = true;
      break;
    case 'F':
    case 'N':
    case '0':
      b = false;
      break;
    default:
      error = PARSE_UNKNOWN;
      b = defaults;
  }

  return b;
}


// special function to get the working mode

int get_working_mode( int argc, char *argv[] )
{
  for (int i=1;i<argc;i++)
  {
    if ( std::strcmp( argv[i], "-m" ) == 0 )
    {
      i++;
      if ( i == argc )
      {
        std::cout << "Warning: no mode parameter given! Returning default values!" << std::endl;
        #ifdef IS_EUCLID_ENVIRONMENT
        return WORKING_MODE_EUCLID;
        #else
        return WORKING_MODE_FITS;
        #endif
      }
      else
      {
        std::string s = argv[i];

        if ( s == "FITS" )
        {
          return WORKING_MODE_FITS;
        }
        if ( s == "ACS" )
        {
          return WORKING_MODE_ACS;
        }
        if ( s == "EUCLID" )
        {
          return WORKING_MODE_EUCLID;
        }
      }
    }
  }
  #ifdef IS_EUCLID_ENVIRONMENT
  return WORKING_MODE_EUCLID;
  #else
  return WORKING_MODE_FITS;
  #endif
}
