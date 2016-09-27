/* (C) Copyright 2013 by Oliver Cordes
        - ocordes ( at ) astro ( dot ) uni-bonn ( dot ) de


    This file is part of artic-project.

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

/* image_slice.hh

   written by: Oliver Cordes 2016-05-09
   changed by: Oliver Cordes 2016-09-27

*/

#ifndef __image_slice_h

#define __image_slice_h 1

#include <cstddef>
#include <string>
#include <exception>

// constants for the slicing orientation
#define image_forward   false
#define image_reverse   true
#define image_readout_x true
#define image_readout_y false


namespace std
{
  class image_slice;
}





class std::image_slice {
private:
  long pos;
  long width;
  long height;
  long offs;
  bool rotate;
  bool direction;

  long begin_pos;
  long end_pos;

  long stride;
public:
  image_slice();
  image_slice( size_t w, size_t h, size_t o, bool rot, bool dir );

  void reset( size_t col );

  long operator*() { return (size_t) pos; }

  long & operator++();
  long operator++( int );
  bool operator!= (  const image_slice & );

  image_slice begin();
  image_slice end();

};

#endif
