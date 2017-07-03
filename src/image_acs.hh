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

/* image_acs.hh

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2017-06-05
*/

#ifndef __image_acs_h

#define __image_acs_h 1


#include "image.hh"
#include "params.hh"
#include "params_acs.hh"

#include <CCfits/CCfits>


class acs_image : public image
{
  public:
    acs_image( int argc, char *argv[] );
    virtual ~acs_image();
    virtual int clock_charge_prepare( void );

    double  date2double( std::string & s );
};


#endif
