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

/* image_base.hh

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2015-01-05

   $Id: image_base.hh 703 2015-01-05 18:22:59Z ocordes $

*/

#ifndef __image_base_h

#define __image_base_h 1


#define image_type_rows 0   // image is stored in rows (default)
#define image_type_cols 1   // image is stored in columns

// Defines a basic type for images as a template

template < typename Pixel_Type > class image_base {
public:
  image_base();                                               // standard conteructor
  image_base( std::size_t N );                                // size constructor
  image_base( const image_base & );                           // copy constructor
  ~image_base();                                              // destructor

  // operators
  image_base<Pixel_Type> & operator=( const image_base & a ); // = operator
private:
  int size;                                                   // number of pixels
  int width;                                                  // image width
  int height;                                                 // image height
  int type;                                                   // pixel organisation
  Pixel_Type *image;                                          // pointer to the pixels
};


#endif
