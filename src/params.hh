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

/* param.hh

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2016-10-24

   $Id$

*/

#ifndef __params_h

#define __params_h 1


#include <cstring>

#include <string>
#include <valarray>
#include <vector>

#include "image_slice.hh"
#include "strstr.hh"

// define the implemented working modes

#define WORKING_MODE_FITS   1
#define WORKING_MODE_ACS    2
#define WORKING_MODE_EUCLID 3


// define the parse-results

#define PARSE_OK            0
#define PARSE_UNKNOWN       1
#define PARSE_ERROR         100


// algorithm definitions
#define ALGORITHM_CLASSIC  1
#define ALGORITHM_NEO      2
#define ALGORITHM_NEO2     3


// Defines a real type for images


class params {
protected:
  void load_config( std::string filename );
public:
  // CTI variables
  unsigned  int            algorithm;
  double                   well_depth;
  double                   well_notch_depth;
  double                   well_fill_power;
  double                   empty_trap_limit;
  double                   empty_trap_limit_neo2;
  bool                     check_empty_traps;
  unsigned int             n_iterations;
  int                      express;
  int                      n_levels;
  bool                     unclock;
  int                      readout_offset;
  bool                     dark_mode;
  std::valarray<long>      xrange;
  std::valarray<long>      yrange;
  int                      n_species;
  std::valarray<double>    trap_density;
  std::valarray<double>    trap_lifetime;
  bool                     cut_upper_limit;
  double                   upper_limit;

  // image variables
  bool                     rotate;
  bool                     direction;
  bool                     force;

  // standard variables
  std::string              working_mode;
  std::string              config_filename;
  std::vector<std::string> config_entries;


  params();
  virtual ~params();

  virtual void parse_args( std::string, std::string, int & );
  virtual void check_params ( void );

  void set_args(  int *argc, char **argv[] );

  std::valarray<double> str2array( std::CString );
  std::valarray<long> str2array_long( std::CString );

  bool  tobool( std::string & );
};


int get_working_mode( int argc, char *argv[] );


#endif
