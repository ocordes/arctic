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

/* param_fits.hh

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2017-03-06
*/

#ifndef __params_fits_h

#define __params_fits_h 1


#include <cstring>

#include <string>
#include <valarray>


#include "params.hh"


class params_fits : public params {
public:
  // variables
  int     static_trap_definitions;
  // functions
  params_fits();
  void parse_args_image( std::string &, std::string &, int & );
  void parse_args_cte( std::string &, std::string &, int & );
  void parse_args_algorithm_basics( std::string &, std::string &, int & );
  void parse_args_algorithm( std::string &, std::string &, int & );
  void parse_args_traps( std::string &, std::string &, int & );
  void parse_args_imagedir( std::string &, std::string &, int & );
  virtual void parse_args( std::string & key, std::string & val, int & error );
  virtual void check_params ( void );
};


void syntax_fits( void );

#endif
