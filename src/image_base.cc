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

/* image_base.cc

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2015-01-05

   $Id: image_base.cc 703 2015-01-05 18:22:59Z ocordes $

*/

#include <cstdlib>
#include <cstring>

#include "image_base.hh"

template <typename Pixel_Type>
image_base <Pixel_Type>::image_base()
{
  image = NULL;
}


template <typename Pixel_Type>
image_base <Pixel_Type>::image_base( std::size_t N )
{ 
  image = (Pixel_Type*) malloc( N * sizeof( Pixel_Type ) );
  size  = N;
  width = size;
  height = 1;
  type = image_type_rows;
}


template <typename Pixel_Type>
image_base <Pixel_Type>::image_base( const image_base & a )
{
  if ( image != NULL )
    free( image );

  // copy image data and meta data
  image  = (Pixel_Type*) malloc( a.size * sizeof( Pixel_Type ) );
  std::memcpy( image, a.image, a.size );
  size   = a.size;
  width  = a.width;
  height = a.height;
  type   = a.type;
}



template <typename Pixel_Type>
image_base <Pixel_Type>::~image_base()
{
  if ( image != NULL )
    free( image );
}


template <typename Pixel_Type>
image_base<Pixel_Type> & image_base<Pixel_Type>::operator=( const image_base & a )
{
  return *this;
}
