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

/* image_euclid.hh

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2017-06-01
*/

#ifndef IMAGE_EUCLID_H

#define IMAGE_EUCLID_H 1


#include "image.hh"
#include "params_euclid.hh"


#include <CCfits/CCfits>


class euclid_image : public image{
public:
  euclid_image( int argc, char *argv[] );
  virtual ~euclid_image();
  virtual int  clock_charge_prepare( void );
};


#endif
