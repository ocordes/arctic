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
   changed by: Oliver Cordes 2016-10-17

   $Id$

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

  // default allocate image_data object
  image_data = std::shared_ptr<std::valarray<double>>( new std::valarray<double> );

  // debug
  //for (int i =1;i<argc;i++)
  //  std::cout << i << " " << argv[i] << std::endl;

  if ( argc < 2 )
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


int euclid_image::clock_charge( void )
{
  // check the image UNITS

  std::string bunit = readkey_string( FITS_image->pHDU(), "BUNIT" );

  for (unsigned int i=0;i<bunit.length();i++) bunit[i] = toupper( bunit[i] );

  if ( bunit == "ELECTRONS" )
    {
      // everything is OK
    }
  else
    {
      if ( bunit == "ELECTRONS/S" )
        {
          // needs to convert
          electrons_per_sec = true;

          // get the exposure time

          double exptime = readkey_double( FITS_image->pHDU(), "EXPTIMEE" );

          if ( std::isnan( exptime ) )
            {
              output( 1, "Warning: EXPTIME doesn't exist! Assume exptime=1s!\n" );
              exptime = 1.0;
            }
          if ( sci_mode_dark )
            {
              double mexptime = readkey_double( FITS_image->pHDU(), "MEANEXP" );

              if ( std::isnan( mexptime ) )
                {
                  output( 1, "Warning: MEANEXP doesn't exist, leaving exptime untouched!\n" );
                }
              else
                exptime = mexptime;
            }

          // recreate electrons from electrons/s
          (*image_data) *= exptime;

        }
      else
        {
          // wrong in general

          std::cout << "WARNING: Image UNITS are not in ELECTRONS or ELECTRONS/S!" << std::endl;
          //std::cout << "Image UNITS are not in ELECTRONS or ELECTRONS/S! Program aborted!" << std::endl;
          //return 1;
        }
    }

  // code adopted from the ACS part


  std::shared_ptr<std::valarray<long>> xrange( new std::valarray<long> ( parameters->xrange ) );
  std::shared_ptr<std::valarray<long>> yrange( new std::valarray<long> ( parameters->yrange ) );

  // create a CTE obejct with the parameter class
  cte_image cte( parameters );

  // do the unclock thing ...
  cte.clock_charge( image_data, image_width, image_height,
                    (*xrange), (*yrange) );

  return 0;
}
