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

/* params.cc

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2016-03-09

   $Id: params.cc 975 2016-03-09 13:08:56Z ocordes $

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

#include "output.hh"
#include "params.hh"
#include "strstr.hh"



params::params()
{
  neo_algorithm     = false;
  neo_algorithm2    = false;
  well_depth        = 84700;     
  well_notch_depth  = 1e-9;
  well_fill_power   = 0.478;
  check_empty_traps = true;
  empty_trap_limit  = 1e-11;
  n_iterations      = 1;
  express           = 1;
  n_levels          = 10000;
  unclock           = true;
  dark_mode         = false;
  readout_offset    = 0;
  xrange            = { 0, LONG_MAX };
  yrange            = { 0, LONG_MAX };
  trap_density      = { 0.0 };
  trap_lifetime     = { 0.0 };

  cut_upper_limit   = false;
  upper_limit       = 65536.-3;

  // only for reading parameters, not used in the CTE code
  n_species         = 0;

  config_filename   = "";
  output( 11, "params::params()\n" );
}


params::~params()
{
  // std::cout << "params::~params()" << std::endl;
}


void params::load_config( std::string filename )
{
  output( 1, "Loading config file '%s' ...\n", filename.c_str() );

  std::ifstream f;

  std::CString  s;

  f.open( filename );

  if ( f.is_open() )
    {
      while ( getline(f, s) )
	{
	  // ignore empty lines and comments
	  if ( ( s == "" ) || ( s[0] == '#' ) )
	    {
	      continue;
	    }

	  // clean spaces
	  s = s.clean_white();
	  
	  //std::cout << s << std::endl;

	  std::CString key = s.strtok( true, "=#" );
	  std::CString val = s.strtok( false, "=#" );

	  // convert key into upper cases
	  key = key.toupper();

	  //std::cout << "key=" << key << " val=" << val << std::endl;
	  parse_args( key, val );
	}
      f.close();
    }
  else
    std::cout << "Can't open config file '" << filename << "'! Config file will be ignored!" << std::endl;
  
  
  output( 1, "Done.\n" );
}


// use this virtual function only for declaration
// it schould never be called!!!

void params::parse_args( std::string key, std::string val )
{
  throw "Not implemented!";
}



void params::set_args( int *argc, char **argv[]  )
{
  int nargc = (*argc);
  int outp  = 1;

  int i = 1; // the first argument is always the program name ;-)

  //std::cout << (*argc) << std::endl;
  while( i < nargc )
    {
      //std::cout << (*argv)[i] << std::endl;

      // check if this an option
      if ( (*argv)[i][0] == '-' )
	{
	  // read at least one argument
	  (*argc)--;

	  if ( std::strcmp( (*argv)[i], "-c" ) == 0 )
	    {
	      (*argc)--;
	      i++;
	      std::string s = (*argv)[i];
	      load_config( s );
	      i++;
	      continue;
	    }
	  if ( ( std::strcmp( (*argv)[i], "-d" ) == 0 ) || (  std::strcmp( (*argv)[i], "-m" ) == 0 ) )
	    {
	      // ignore debug and mode definitions
	      if ( (i+1) < nargc )
		{
		  (*argc)--;
		  i++;
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
  
	  parse_args( key, val );
	} // end of if (*argv)[0]  == '-' 
      else
	{
	  (*argv)[outp] = (*argv)[i];
	  (*argv)[i] = NULL;
	  outp++;
	}
      
      i++;
    }
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


std::valarray<long> params::str2array_long( std::CString s )
{
  std::vector<long> temp_vec;

  std::CString p = s.strtok( true, "," );
  while ( p != "" )
    {
      temp_vec.push_back( atoi( p.c_str() ) );
      p = s.strtok( false, "," );
    }

  return std::valarray<long> ( &temp_vec[0], temp_vec.size() );
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
	      return WORKING_MODE_FITS;
	    }
	  else
	    {
	      std::string s = argv[i];

	      if ( s == "FITS" )
		return WORKING_MODE_FITS;
	      if ( s == "ACS" )
		return WORKING_MODE_ACS;
	      if ( s == "EUCLID" )
		return WORKING_MODE_EUCLID;
	    }
	}
	
    }
  return WORKING_MODE_FITS;
}



bool params::tobool( std::string & val )
{
  int b = false;

  std::CString s = val;
  s = s.toupper();

  switch( s[0] )
    {
    case '\0':
      break;
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
    }

  return b;
}
