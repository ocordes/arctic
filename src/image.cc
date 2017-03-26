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
   changed by: Oliver Cordes 2017-03-13

   $Id$

*/


#include <cstdlib>
#include <cstring>

#include <sys/param.h>
#include <unistd.h>

#include <iostream>

#include <vector>


#include "config.h"
#include "cte_image.hh"
#include "cte_image_classic.hh"
#include "cte_image_neo.hh"
#include "cte_image_watermark.hh"
#include "image.hh"
#include "output.hh"


#include <CCfits/CCfits>

using namespace CCfits;


// some aditional C++ functions

std::string get_working_path()
{
   char temp[MAXPATHLEN];
   return ( getcwd(temp, MAXPATHLEN) ? std::string( temp ) : std::string("") );
}



image::image( int argc, char *argv[] )
{
  prgname      = "";
  for (int i=0;i<argc;++i)
    if ( prgname == "" )
       prgname = std::string( argv[i] );
    else
       prgname = prgname + " " + std::string( argv[i] );

  infilename   = "";
  outfilename  = "";
  image_width  = 0;
  image_height = 0;

  FITS_image   = NULL;
  image_data   = NULL;

}


image::~image()
{
}


void image::fits_info( void )
{
  std::string       line;
  std::stringstream ss;

  if ( is_debug( 10 ) )
  {
    ss << (*FITS_image);

    while ( getline( ss, line ) )
    {
      output( 10, "%s\n", line.c_str() );
    }
  }
}



int image::read_file( void )
{
  output( 1, "Reading file %s\n", infilename.c_str() );

  if ( debug_level > 1 )
    FITS::setVerboseMode(true);

  try {
    FITS_image = std::unique_ptr<FITS> ( new FITS( infilename , Read, true) );

    // write some information about the image
    fits_info();


    // read all user-specifed, coordinate, and checksum keys in the image
    FITS_image->pHDU().readAllKeys();

    // check tghe number of axis and check if extensions are available

    if ( ( FITS_image->pHDU().axes() < 2 ) && ( FITS_image->pHDU().extend() ) )
    {
      std::cerr << "File '" << infilename << "' is a multiple extension FITS";
      std::cerr << " file which cannot be processed by arctic!" << std::endl;
      return 1;
    }


    // read the image data
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


void image::update_header_serial( PHDU & hdu )
{
  // used for writing key words
  std::string key;
  std::string comm;

  // special output for serial- = x- clocking

  hdu.addKey( "CTE_SNTR", parameters->trap_density.size(), "Number of charge trap species" );
  for (unsigned int i=0;i<parameters->trap_density.size();i++)
    {
      key  = "CTE_TS" + std::to_string( i+1 ) + "D";
      comm = "Density of " + std::to_string( i+1 ) + ". charge trap species [pixel^-1]";
      hdu.addKey( key, parameters->trap_density[i], comm );
      key  = "CTE_TS" + std::to_string( i+1 ) + "T";
      comm = "Decay halflife of " + std::to_string( i+1 ) + ". charge trap species";
      hdu.addKey( key, parameters->trap_lifetime[i], comm );
    }
  hdu.addKey( "CTE_SNLE", parameters->n_levels, "N_levels parameter in ClockCharge code" );
  if ( parameters->direction )
    hdu.addKey( "CTE_SDIR",  "REVERSE", "READOUT direction" );
  else
    hdu.addKey( "CTE_SDIR",  "FORWARD", "READOUT direction" );
}


void image::update_header_parallel( PHDU & hdu )
{
  // used for writing key words
  std::string key;
  std::string comm;

  // special output for parallel- = y- clocking

  hdu.addKey( "CTE_PNTR", parameters->trap_density.size(), "Number of charge trap species" );
  for (unsigned int i=0;i<parameters->trap_density.size();i++)
    {
      key  = "CTE_TP" + std::to_string( i+1 ) + "D";
      comm = "Density of " + std::to_string( i+1 ) + ". charge trap species [pixel^-1]";
      hdu.addKey( key, parameters->trap_density[i], comm );
      key  = "CTE_TP" + std::to_string( i+1 ) + "T";
      comm = "Decay halflife of " + std::to_string( i+1 ) + ". charge trap species";
      hdu.addKey( key, parameters->trap_lifetime[i], comm );
    }
  hdu.addKey( "CTE_PNLE", parameters->n_levels, "N_levels parameter in ClockCharge code" );
  if ( parameters->direction )
    hdu.addKey( "CTE_PDIR",  "REVERSE", "READOUT direction" );
  else
    hdu.addKey( "CTE_PDIR",  "FORWARD", "READOUT direction" );
}


void image::update_header( PHDU & hdu)
{
  // add some keywords
  time_t systime = time( NULL );
  std::string c = "CTE model applied " + std::string( ctime( & systime ) );
  hdu.addKey( "CTE_VERS", std::string( VERSION ), c );
  hdu.addKey( "CTE_ALGO", algorithm_names[parameters->algorithm], "Name of the algorithm used" );
  hdu.addKey( "CTE_MODE", parameters->working_mode, "Working mode for getting trap information" );
  hdu.addKey( "CTE_ITER", parameters->n_iterations, "Number of iterations used during CTE correction" );
  hdu.addKey( "CTE_WELD", parameters->well_depth, "Assumed pixel well depth [electrons]" );
  hdu.addKey( "CTE_WELN", parameters->well_notch_depth, "Assumed notch depth [electrons]" );
  hdu.addKey( "CTE_WELP", parameters->well_fill_power, "Power law controlling filling of pixel well" );
  hdu.addKey( "CTE_NTRS", parameters->trap_density.size(), "Number of charge trap species" );

  // used for writing key words
  std::string key;
  std::string comm;

  for (unsigned int i=0;i<parameters->trap_density.size();i++)
    {
      key  = "CTE_TR" + std::to_string( i+1 ) + "D";
      comm = "Density of " + std::to_string( i+1 ) + ". charge trap species [pixel^-1]";
      hdu.addKey( key, parameters->trap_density[i], comm );
      key  = "CTE_TR" + std::to_string( i+1 ) + "T";
      comm = "Decay halflife of " + std::to_string( i+1 ) + ". charge trap species";
      hdu.addKey( key, parameters->trap_lifetime[i], comm );
    }
  hdu.addKey( "CTE_NLEV", parameters->n_levels, "N_levels parameter in ClockCharge code" );

  if ( parameters->rotate )
    hdu.addKey( "CTE_READ",  "SERIAL", "Readout modus" );
  else
    hdu.addKey( "CTE_READ",  "PARALLEL", "Readout modus" );

  if ( parameters->direction )
    hdu.addKey( "CTE_DIR",  "REVERSE", "READOUT direction" );
  else
    hdu.addKey( "CTE_DIR",  "FORWARD", "READOUT direction" );


  if ( parameters->rotate )
    update_header_serial( hdu );
  else
    update_header_parallel( hdu );


      // write HISTORY strings to FITS file

      hdu.writeHistory( "========================================================================" );
      hdu.writeHistory( "artic Version "+ std::string( VERSION) );
      hdu.writeHistory( "RUN: " + std::string( ctime( & systime ) ) );
      hdu.writeHistory( "CMD: " + prgname );
      hdu.writeHistory( "CWD: " + get_working_path() );

      if ( parameters-> config_filename != "" )
      {
        hdu.writeHistory( "Config File: " + parameters->config_filename );

        std::vector<std::string>::iterator iter = parameters->config_entries.begin();
        std::vector<std::string>::iterator end = parameters->config_entries.end();
        while(iter != end)
        {
          hdu.writeHistory( " " + (*iter) );
          ++iter;
        }
      }

      hdu.writeHistory( "========================================================================" );
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


  // new reference for HDU writing
  PHDU& hdu = pFits->pHDU();

  // update the FITS header
  update_header( hdu );

  // The function PHDU& FITS::pHDU() returns a reference to the object representing
  // the primary HDU; PHDU::write( <args> ) is then used to write the data.

  long  fpixel(1);  // ???? what the heck is this useful ...

  hdu.write( fpixel, nelements, (*image_data) );

  output( 1, "Done.\n" );
}


int image::clock_charge_prepare( void )
{
  throw "Not implemented!";

  return 0;
}


int image::clock_charge( void )
{
  int ret = clock_charge_prepare();

  if ( ret != 0 ) return ret;


  // create a CTE obejct with the parameter class
  cte_image *cte;

  switch( parameters->algorithm )
  {
    case ALGORITHM_CLASSIC:
      cte = new cte_image_classic( parameters );
      break;
    case ALGORITHM_NEO:
      cte = new cte_image_neo( parameters );
      break;
    case ALGORITHM_NEO2:
      cte = new cte_image_watermark( parameters );
      break;
    default:
      cte = new cte_image_neo( parameters );
      break;
  }

  // setup all variables inside the cte structure
  cte->setup( image_width, image_height );

    // do the unclock thing ...
  cte->clock_charge( image_data );

  // free the cte image
  delete cte;

  return 0;
}
