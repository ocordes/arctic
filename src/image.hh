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

/* image.hh

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2017-05-11

   $Id$

*/

#ifndef __image_h

#define __image_h 1


#include "params.hh"

#include <CCfits/CCfits>

using namespace CCfits;


// Defines the simple abstract image class


class image{
public:
  image( int argc, char *argv[] );
  virtual     ~image();
  void         fits_info( void );
  virtual int  read_file( void );
  virtual void write_file( void );
  virtual int  clock_charge_prepare( void );
  int          clock_charge( void );

  template <typename T>  T readkey( PHDU & pHDU, std::string key );

private:
  void         update_header( PHDU & hdu);
  void         update_header_serial( PHDU & hdu );
  void         update_header_parallel( PHDU & hdu );


protected:
  int          correct_units( void );
  bool                                   sci_mode_dark;
  bool                                   electrons_per_sec;
  double                                 exptime;
  
  std::string                            prgname;
  std::shared_ptr<params>                parameters;
  std::string                            infilename;
  std::string                            outfilename;

  std::unique_ptr<FITS>                  FITS_image;

  std::valarray<double>                  image_data;

  long                                   image_width;
  long                                   image_height;
};

template <typename T>
T image::readkey( PHDU & pHDU, std::string key )
{
  T val;

  try {
    pHDU.readKey( key, val );
  }
  catch (FitsException&)
    {
      std::cerr << " Fits Exception Thrown by readkey function" << std::endl;
      std::cerr << " Can't read the key " << key << " or key is not a string!" <<  std::endl;
    }

  return val;
}


#endif
