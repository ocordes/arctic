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
   changed by: Oliver Cordes 2017-07-12
*/

#ifndef __image_h

#define __image_h 1

#include <cmath>

#include "params.hh"

#include <CCfits/CCfits>


// Defines the simple abstract image class


class image{
public:
  image( int argc, char *argv[] );
  virtual     ~image();
  void         fits_info( void );
  virtual int  read_file( void );
  virtual int  write_file( void );
  virtual int  clock_charge_prepare( void );
  int          clock_charge( void );
  int          correct_units( void );

  template <typename T>  T readkey( std::string, T );

  std::string                            prgname;
  bool                                   sci_mode_dark;
  bool                                   electrons_per_sec;
  double                                 exptime;

  std::shared_ptr<params>                parameters;
  std::string                            infilename;
  std::string                            outfilename;

  std::unique_ptr<CCfits::FITS>          FITS_image;

  std::valarray<double>                  image_data;

  long                                   image_width;
  long                                   image_height;
private:
  void         update_header( CCfits::PHDU & hdu);
  void         update_header_serial( CCfits::PHDU & hdu );
  void         update_header_parallel( CCfits::PHDU & hdu );
};


//readkey must have a fallback in the case that a key doesn't exist in
//the FITS header.

template <typename T>
T image::readkey( std::string key, T error_default )
{
  T val;

  try {
    FITS_image->pHDU().readKey( key, val );
  }
  catch (CCfits::FitsException&)
    {
      std::cerr << " Fits Exception Thrown by readkey function" << std::endl;
      std::cerr << " Can't read the key " << key << " or key is not a string!" <<  std::endl;
      val = error_default;
    }


  return val;
}


std::string get_working_path( void );

#endif
