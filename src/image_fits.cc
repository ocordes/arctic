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

/* image.cc

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2017-02-25

   $Id$

*/

//#define _GLIBCXX_USE_CXX11_ABI 0

#include <cstdlib>
#include <cstring>

#include <iostream>

#include <CCfits/CCfits>

#include "config.h"
#include "cte_image.hh"
#include "cte_image_classic.hh"
#include "cte_image_neo.hh"
#include "cte_image_watermark.hh"
#include "image.hh"
#include "image_fits.hh"
#include "output.hh"
#include "params_fits.hh"


using namespace CCfits;

fits_image::fits_image( int argc, char *argv[] ) : image( argc, argv )
{
  // alloctae parameter object
  parameters  = std::shared_ptr<params_fits>( new params_fits() );

  // initialize with command line parameters
  parameters->set_args( &argc, &argv);

  // default allocate image_data object
  image_data = std::shared_ptr<std::valarray<double>>( new std::valarray<double> );


  if ( argc < 2 )
    {
      output( 1, "Program needs <INPUTFILE> <OUTPUTFILE>! Abort\n" );
      throw "Not enough arguments!";
    }

  // copy filename parameters
  infilename = argv[1];
  outfilename = argv[2];
}


fits_image::~fits_image()
{
}


int fits_image::clock_charge_prepare( void )
{
  return 0;
}
