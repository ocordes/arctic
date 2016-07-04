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

/* image.hh

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2016-07-04

   $Id: image.hh 917 2015-12-02 10:00:30Z ocordes $

*/

#ifndef __image_h

#define __image_h 1


#include "params.hh"

#include <CCfits/CCfits>

using namespace CCfits;


// Defines the simple abstract image class


class image{
public:
  image();
  virtual ~image();
  virtual int  read_file( void );
  virtual void write_file( void );
  virtual int  clock_charge( void );

  int         readkey_int( PHDU & pHDU, std::string key );
  double      readkey_double( PHDU & pHDU, std::string key );
  std::string readkey_string( PHDU & pHDU, std::string key );
protected:
  std::shared_ptr<params>                parameters;
  std::string                            infilename;
  std::string                            outfilename;

  std::unique_ptr<FITS>                  FITS_image;

  std::shared_ptr<std::valarray<double>> image_data;

  long                                   image_width;
  long                                   image_height;
};


#endif
