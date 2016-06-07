/* (C) Copyright 2013 by Oliver Cordes
        - ocordes ( at ) astro ( dot ) uni-bonn ( dot ) de


    This file is part of artic-project.

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

/* image_slice.cc

   written by: Oliver Cordes 2016-05-09
   changed by: Oliver Cordes 2016-05-11

*/

#include <cstddef>

#include <string>

#include <iostream>

#include "image_slice.hh"


std::image_slice::image_slice()
{
  width     = 0;
  height    = 0;
  offs      = 0;

  rotate    = false;
  direction = image_forward;

  // positional variales
  pos       = 0;
  begin_pos = 0;
  end_pos   = 0;

  stride    = 0;
}

std::image_slice::image_slice( size_t w, size_t h, size_t o, bool rot, bool dir )
{
  width     = w;
  height    = h;
  offs      = o;

  rotate    = rot;
  direction = dir;

  // positional variales
  pos       = 0;
  begin_pos = 0;
  end_pos   = 0;

  stride    = 0;
}


// image_slice::reset prepares all internal variables for the
// iteration

void std::image_slice::reset( size_t col )
{
  if ( rotate )
    {
      // all variables for the rotation

      // image will be accessed in x direction

      if ( offs >= width )
	throw std::slice_exception( std::string( "Offset larger than image width!" ) );
      
      if ( direction )
	{
	  // image reverse
	  begin_pos = (col+1)*width - 1 - offs;
	  end_pos   = col*width -1;
	  stride    = -1;
	}
      else
	{
	  // image_forward
	  begin_pos = col*width + offs;
	  end_pos   = (col+1)*width;
	  stride    = 1;
	}
    }
  else
    {
      // standard operation

      // image will be accessed in y direction

      if ( offs >= height )
	throw std::slice_exception( std::string( "Offset larger than image height!" ) );
      
      if ( direction )
	{
	  // image reverse
	  begin_pos = (height-1-offs)*width + col;
	  end_pos   = col - width;
	  stride    = -width;
	}
      else
	{
	  // image_forward
	  begin_pos = offs*width + col;     // first eleemt in the slice
	  end_pos   =  height*width + col;  // outside the def, which is okay for the last element ;-)
	  stride    = width;
	}
    }
  
  pos = begin_pos;
  //throw std::slice_exception( std::string("Reset failed!") );

  // std::cout << begin_pos << " " << end_pos << " " << stride << std::endl;
}


long & std::image_slice::operator++( void )
{
  pos += stride;
  
  if ( stride > 0 )
    {
      if ( pos > end_pos )
	throw std::slice_exception( std::string( "Out of bounds (end_pos)!" ) );
    }
  else
    {
      //std::cout << "pos=" << pos << " end_pos=" << end_pos << std::endl;
      if ( pos < end_pos )
	throw std::slice_exception( std::string( "Out of bounds (end_pos)!" ) );
    }
  return pos;
}

long std::image_slice::operator++(  int )
{
  long t;

  t = pos;
  pos += stride;

  if ( stride > 0 )
    {
      if ( pos > end_pos )
	throw std::slice_exception( std::string( "Out of bounds (end_pos)!" ) );
    }
  else
    {
      //std::cout << "pos=" << pos << " end_pos=" << end_pos << std::endl;
      if ( pos < end_pos )
	throw std::slice_exception( std::string( "Out of bounds (end_pos)!" ) );
    }
  
  return t;
}



std::image_slice std::image_slice::begin( void )
{
  std::image_slice t;

  t = (*this);
  t.pos = t.begin_pos;

  return t;
}


std::image_slice std::image_slice::end( void )
{
  std::image_slice t;

  t = (*this);
  t.pos = t.end_pos;

  return t;
}



// friend overloaded operator

bool std::image_slice::operator!= ( const std::image_slice & p )
{
  //std::cout << "hi" << std::endl;
  return p.pos != pos ;
}
