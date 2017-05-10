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

/* arctic.cc

   written by: Oliver Cordes 2013-02-05
   changed by: Oliver Cordes 2017-02-20

   $Id$

*/

#include <cstdlib>
#include <cstring>

#include <iostream>


#include "config.h"

#include "image.hh"
#include "image_acs.hh"
#include "image_euclid.hh"
#include "image_fits.hh"

#include "output.hh"


void help_check( int argc, char *argv[] )
{
  for (int i=1;i<argc;++i)
  {
      if ( ( std::strcmp( argv[i], "-h" ) == 0 )
        || ( std::strcmp( argv[i], "-?" ) == 0 )
        || ( std::strcmp( argv[i], "--help" ) == 0 ) )
        {
          // print the syntax information
          std::cout << "SYNTAX: arctic -m MODE <INFILE> <OUTFILE> <OPTIONS>" << std::endl;
          std::cout << "  MODE is one of the following:" << std::endl;
          std::cout << "    FITS   :  working on simple FITS files (default)" << std::endl;
          std::cout << "    ACS    :  working on ACS files, using the observation date" << std::endl;
          std::cout << "              for setting the trap specification" << std::endl;
          std::cout << "    EUCLID :  working on EUCLID files in a special environment" << std::endl;
          std::cout << std::endl;
          std::cout << "  OPTIONS are special for different working modes:" << std::endl;
          std::cout << "   FITS options:" << std::endl;
          syntax_fits();
          std::cout << std::endl;
          std::cout << "   ACS options (inherit all FITS options):" << std::endl;
          syntax_acs();
          std::cout << std::endl;
          std::cout << "   EUCLID options (inherit all FITS options):" << std::endl;
          syntax_euclid();
          std::cout << std::endl;

          exit( 0 );
        }
  }
}


int main( int argc, char *argv[])
{
  std::cout << argv[0] << " Version " << VERSION << " build " << BUILD;
#ifdef IS_EUCLID_ENVIRONMENT
    std::cout << "(Euclid)";
#endif
  std::cout << std::endl;

  // check for help and syntax parameter
  help_check( argc, argv );

  // parse for debug parameter and inititialize the debug/output module
  debug_init( argc, argv );

  // parse for working mode parameter for special param objects!
  int working_mode = get_working_mode( argc, argv );

  // params_init( &argc, &argv );
  // params parameters( &argc, &argv );

  image *im = NULL;

  try {
    std::cout << "Working in ";
    switch ( working_mode )
    {
      case WORKING_MODE_FITS:
        std::cout << "FITS mode!" << std::endl;
        im = new fits_image( argc, argv );
        break;
      case WORKING_MODE_ACS:
        std::cout << "ACS mode!" << std::endl;
        im = new acs_image( argc, argv );
        break;
      case WORKING_MODE_EUCLID:
        std::cout << "Euclid mode!" << std::endl;
        im = new euclid_image( argc, argv );
        break;
      default:
        std::cout << "Standard FITS mode!" << std::endl;
        im = new fits_image( argc, argv );
        break;
    }
  }
  catch ( char const *s )
  {
    std::cerr << s << " Program aborted!" <<  std::endl;
    exit( 1 );
  }


  if ( im->read_file() != 0 )
  {
    std::cerr << "Program aborted!" <<  std::endl;
    exit( 1 );
  }

  if ( im->clock_charge() == 0 )
  {
    // save file only of the CTE correction was run sccessfully
    im->write_file();
  }


  // free image
  delete im;

  return 0;
}
