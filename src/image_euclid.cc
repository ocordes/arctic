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

/* image_euclid.cc

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2016-05-10

   $Id: image_euclid.cc 998 2016-05-10 06:20:37Z ocordes $

*/


#include <cstdlib>
#include <cstring>

#include <iostream>

#include <CCfits/CCfits>

#include "config.h"
#include "cte_image_euclid.hh"
#include "image.hh"
#include "image_euclid.hh"
#include "output.hh"
#include "params_euclid.hh"


using namespace CCfits;

euclid_image::euclid_image( int argc, char *argv[] ) : image()
{
  parameters  = std::shared_ptr<params_euclid>( new params_euclid() );
  parameters->set_args( &argc, &argv);

  // debug
  //for (int i =1;i<argc;i++)
  //  std::cout << i << " " << argv[i] << std::endl;
  
  if ( argc < 2 )
    {
      output( 1, "Program needs <INPUTFILE> <OUTPUTFILE>! Abort\n" );
      throw "Not enough arguments!";
    }

 
  infilename = argv[1];
  outfilename = argv[2];

  sci_mode_dark = false;

  electrons_per_sec = false;
}


euclid_image::~euclid_image()
{
}



int euclid_image::read_file( void )
{
  if ( debug_level > 1 )
    FITS::setVerboseMode(true);

  try {
    //std::auto_ptr<FITS> pInfile(new FITS( infilename ,Read,true));  // deprecated
    //std::unique_ptr<FITS> pInfile(new FITS( infilename ,Read,true)); // old implementation

    FITS_image = std::unique_ptr<FITS> ( new FITS( infilename, Read, true) );

    // read all user-specifed, coordinate, and checksum keys in the image
    FITS_image->pHDU().readAllKeys();
    FITS_image->pHDU().read( (*image_data) );

    // this doesn't print the data, just header info.
    // std::cout << *FITS_image << std::endl;



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

  return 0;
}


void euclid_image::write_file( void )
{
  
  std::unique_ptr<FITS> pFits;

  try
    {
      // overwrite existing file if the file already exists.

      const std::string fileName( "!"+outfilename );         // overwrite code!

      // Create a new FITS object, specifying the data type and axes for the primary
      // image. Simultaneously create the corresponding file.

      // this image is unsigned short data, demonstrating the cfitsio extension
      // to the FITS standard.

      // create a new FITS object which we don't need ...
      //pFits.reset( new FITS(fileName , FLOAT_IMG , naxis, naxes ) );

      // make a copy of the old image including the header dedfinitions
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

  //add two keys to the primary header, one long, one complex.

  //long exposure(1500);
  //std::complex<float> omega(std::cos(2*M_PI/3.),std::sin(2*M_PI/3));
  //pFits->pHDU().addKey("EXPOSURE", exposure,"Total Exposure Time");
  //pFits->pHDU().addKey("OMEGA",omega," Complex cube root of 1 ");

  // add some keywords
  time_t systime = time( NULL );
  std::string c = "CTE model applied " + std::string( ctime( & systime ) );
  pFits->pHDU().addKey( "CTE_VERS", VERSION, c );
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


  // PHDU's friend ostream operator. Doesn't print the entire array, just the
  // required & user keywords, and is provided largely for testing purposes [see
  // readImage() for an example of how to output the image array to a stream].


  // std::cout << pFits->pHDU() << std::endl;
  // std::cout << fpixel << std::endl;
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

	  std::cout << "Image UNITS are not in ELECTRONS or ELECTRONS/S! Program aborted!" << std::endl;
	  
	  return 1;
	}
    }

  // code adopted from the ACS part
  
  std::shared_ptr<std::valarray<long>> xrange( new std::valarray<long> ( parameters->xrange ) );
  std::shared_ptr<std::valarray<long>> yrange( new std::valarray<long> ( parameters->yrange ) );
  
  // create a CTE obejct with the parameter class
  cte_image_euclid cte( parameters );

  // do the unclock thing ...
  cte.clock_charge( image_data, image_width, image_height,
		    (*xrange), (*yrange),
		    true );

  
  return 0;
}
