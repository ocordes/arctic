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
   changed by: Oliver Cordes 2016-07-04

   $Id: image.cc 917 2015-12-02 10:00:30Z ocordes $

*/


#include <cstdlib>
#include <cstring>

#include <iostream>


#include "config.h"
#include "image.hh"
#include "output.hh"


#include <CCfits/CCfits>

using namespace CCfits;



image::image()
{
  infilename = "";
  outfilename = "";
  image_width = 0;
  image_height = 0;

  FITS_image = NULL;
  image_data = NULL;
}


image::~image()
{
}



int image::read_file( void )
{
  output( 1, "Reading file %s\n", infilename.c_str() );

  if ( debug_level > 1 )
    FITS::setVerboseMode(true);

  try {
    FITS_image = std::unique_ptr<FITS> ( new FITS( infilename , Read, true) );

    // read all user-specifed, coordinate, and checksum keys in the image
    FITS_image->pHDU().readAllKeys();
    FITS_image->pHDU().read( (*image_data) );

    // read image properties
    image_width = FITS_image->pHDU().axis(0);
    image_height = FITS_image->pHDU().axis(1);

    }
    catch (FitsException&)
    // will catch all exceptions thrown by CCfits, including errors
    // found by cfitsio (status != 0)
    {
      std::cerr << "Cannot open file " << infilename << " !" << std::endl;
      return 1;
    }

  output( 1, "Done.\n" );

  return 0;
}


void image::write_file( void )
{
  std::unique_ptr<FITS> pFits;

  output( 1, "Saving data to '%s' ...\n", outfilename.c_str() );

  try
  {
    // overwrite existing file if the file already exists.

    const std::string fileName( "!"+outfilename );         // overwrite mode!

    // Create a new FITS object, specifying the data type and axes for the primary
    // image. Simultaneously create the corresponding file.

    // this image is unsigned short data, demonstrating the cfitsio extension
    // to the FITS standard.

    // create a new FITS object which we don't need ...
    //pFits.reset( new FITS(fileName , FLOAT_IMG , naxis, naxes ) );

    // make a copy of the old image including the header dedfinitions
    // pFits.reset( new FITS( fileName, (*FITS_image) ) );
    pFits.reset( new FITS( fileName, (*FITS_image) ) );
  }
  catch (FITS::CantCreate)
  {
    // ... or not, as the case may be.
    std::cout << "Error: Can't write to '" << outfilename << "'!" << std::endl;
    return;
  }

  // references for clarity.

  long nelements = image_width * image_height;


  // add some keywords
  time_t systime = time( NULL );
  std::string c = "CTE model applied " + std::string( ctime( & systime ) );
  pFits->pHDU().addKey( "CTE_VERS", std::string( VERSION ), c );
  pFits->pHDU().addKey( "CTE_ITER", parameters->n_iterations, "Number of iterations used during CTE correction" );
  pFits->pHDU().addKey( "CTE_WELD", parameters->well_depth, "Assumed pixel well depth [electrons]" );
  pFits->pHDU().addKey( "CTE_WELN", parameters->well_notch_depth, "Assumed notch depth [electrons]" );
  pFits->pHDU().addKey( "CTE_WELP", parameters->well_fill_power, "Power law controlling filling of pixel well" );
  pFits->pHDU().addKey( "CTE_NTRS", parameters->trap_density.size(), "Number of charge trap species" );
  for (unsigned int i=0;i<parameters->trap_density.size();i++)
    {
      std::string key  = "CTE_TR" + std::to_string( i+1 ) + "D";
      std::string comm = "Density of " + std::to_string( i+1 ) + ". charge trap species [pixel^-1]";
      pFits->pHDU().addKey( key, parameters->trap_density[i], comm );
    }
  for (unsigned int i=0;i<parameters->trap_density.size();i++)
    {
      std::string key  = "CTE_TR" + std::to_string( i+1 ) + "T";
      std::string comm = "Decay halflife of " + std::to_string( i+1 ) + ". charge trap species";
      pFits->pHDU().addKey( key, parameters->trap_lifetime[i], comm );
    }
  pFits->pHDU().addKey( "CTE_MLEV", parameters->n_levels, "N_levels parameter in ClockCharge code" );

  // The function PHDU& FITS::pHDU() returns a reference to the object representing
  // the primary HDU; PHDU::write( <args> ) is then used to write the data.

  long  fpixel(1);  // ???? what the heck is this useful ...

  pFits->pHDU().write(fpixel,nelements, (*image_data) );

  output( 1, "Done.\n" );
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
