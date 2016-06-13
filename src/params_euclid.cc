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

/* params_euclid.cc

   written by: Oliver Cordes 2015-06-18
   changed by: Oliver Cordes 2016-06-13

   $Id: params_euclid.cc 803 2015-06-23 12:56:45Z ocordes $

*/


#include "output.hh"
#include "params_fits.hh"
#include "params_euclid.hh"
#include "strstr.hh"



void params_euclid::parse_args( std::string key, std::string val )
{
  output( 11, "params_euclid::parse_args( key=%s, val=%s)\n", key.c_str(), val.c_str() );
  params_fits::parse_args( key, val );
}


void params_euclid::check_params ( void )
{
}
