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

/* param_acs.hh

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2017-02-28
*/

#ifndef __params_acs_h

#define __params_acs_h 1


#include <cstring>

#include <string>
#include <valarray>


#include "params_fits.hh"


class params_acs : public params_fits {
public:
  // variables

  // functions
  params_acs();
  virtual void parse_args( std::string &, std::string &, int & );

  void calc_trap_config( double date );
  virtual void check_params ( void );
};


void syntax_acs( void );

#endif
