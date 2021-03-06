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

/* image_euclid.cc

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2017-07-17
*/

//#define _GLIBCXX_USE_CXX11_ABI 0

#include <cstdlib>
#include <cstring>

#include <iostream>

#include <CCfits/CCfits>

#include "config.h"
#include "cte_image.hh"
#include "image.hh"
#include "image_euclid.hh"
#include "output.hh"
#include "params_euclid.hh"


using namespace CCfits;

euclid_image::euclid_image( int argc, char *argv[] ) : image( argc, argv )
{
  // alloctae parameter object
  parameters  = std::shared_ptr<params_euclid>( new params_euclid() );
  // initialize with command line parameters
  parameters->set_args( &argc, &argv);


  if ( argc < 3 )
  {
    output( 1, "Program needs <INPUTFILE> <OUTPUTFILE>! Abort\n" );
    throw "Not enough arguments!";
  }

  // copy filename parameters
  infilename = argv[1];
  outfilename = argv[2];
}


euclid_image::~euclid_image()
{
}


int euclid_image::clock_charge_prepare( void )
{
  correct_units();

  return 0;
}
