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

/* image.cc

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2015-12-02

   $Id: image.cc 917 2015-12-02 10:00:30Z ocordes $

*/


#include <cstdlib>
#include <cstring>

#include <iostream>


#include "image.hh"


#include <CCfits/CCfits>

using namespace CCfits;



image::image()
{
}


image::~image()
{
}



int image::read_file( void )
{
  throw "Not implemented!";
}


void image::write_file( void )
{
  throw "Not implemented!";
}


int image::clock_charge( void )
{
  throw "Not implemented!";

  return 0;
}


// readKey functions with CCFits eception handling

int image::readkey_int( PHDU & pHDU, std::string key )
{
  int i;

  try {
    pHDU.readKey( key, i );
  }
  catch (FitsException&)
    {
      std::cerr << " Fits Exception Thrown by readkey function" << std::endl;
      std::cerr << " Can't read the key " << key << " or key is not a int!" <<  std::endl;
      i = 0;
    }

  return i;
}


double image::readkey_double( PHDU & pHDU, std::string key )
{
  double d;

  try {
    pHDU.readKey( key, d );
  }
  catch (FitsException&)
    {
      std::cerr << " Fits Exception Thrown by readkey function" << std::endl;
      std::cerr << " Can't read the key " << key << " or key is not a double!" <<  std::endl;
      d = NAN;
    }

  return d;
}


std::string image::readkey_string( PHDU & pHDU, std::string key )
{
  std::string s;

  try {
    pHDU.readKey( key, s );
  }
  catch (FitsException&)
    {
      std::cerr << " Fits Exception Thrown by readkey function" << std::endl;
      std::cerr << " Can't read the key " << key << " or key is not a string!" <<  std::endl;
    }

  return s;
}
