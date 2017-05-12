/* (C) Copyright 2013,2014,205,2016 by Oliver Cordes
        - ocordes ( at ) astro ( dot ) uni-bonn ( dot ) de

w
    This file is part of arctic project.

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

/* cte_image_classic.cc

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2017-05-12


   $Id$

*/

#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iomanip>

#include "cte_image.hh"
#include "cte_image_classic.hh"
#include "image_slice.hh"
#include "output.hh"
#include "params.hh"


#include <iostream>
#include <memory>


#include <sys/time.h>
#include <sys/resource.h>

// #define __debug

#define debug_precision 15
#define debug_pixel 800

// use to activate the comparison code
//#define compariaon

// endof macros


cte_image_classic::cte_image_classic( std::shared_ptr<params> p )
                                    : cte_image( p )
{
  // initialize variables
  check_empty_traps = parameters->check_empty_traps;
  empty_trap_limit = parameters->empty_trap_limit;
}

#ifdef __debug
void cte_image_classic::print_traps( void )
{
  int i, j;

  std::string s;

  output( 10, "trap array output (n_species=%i, trap_levels=%i):\n",
              parameters->n_species, n_levels_traps );
  for (j=0;j<n_levels_traps;++j)
  {
    int c;
    double d;

    s = "";
    c = 0;
    d = 0.0;
    for (i=0;i<parameters->n_species;++i)
    {
      if ( j > 0 )
      {
        if ( fabs( traps[(j*parameters->n_species)+i] - traps[((j-1)*parameters->n_species)+i] ) < 1e-14 )
        {
          c++;
        }
      }
      std::stringstream str;
      str << std::fixed << std::setprecision( debug_precision ) << traps[(j*parameters->n_species)+i] << " ";
      s += str.str();
      //s += std::to_string( t[(j*n_species)+i] ) + " ";
      d +=  traps[(j*parameters->n_species)+i];
    }

    std::stringstream str;
    str << std::fixed << std::setprecision( debug_precision ) << d;
    s += "= " + str.str();
    //      s += "= " + std::to_string( d );
    if ( c < 3 )
    {
      output( 10, "%05i: %s\n", j, s.c_str() );
    }
  }
}
#endif


void   cte_image_classic::clock_charge_setup( void )
{
  output( 1, "setup\n");
  output( 1, "Using n_levels=%i\n", parameters->n_levels );

  // setup variables
  n_electrons_per_trap = std::valarray<double> ( parameters->trap_density / (double) parameters->n_levels );
  n_electrons_per_trap_total = traps_total / parameters->n_levels;

  output( 10, "Create trap structure ...\n" );
  traps = std::valarray<double>( 0.0, parameters->n_species * parameters->n_levels );

  n_electrons_per_trap_express = n_electrons_per_trap;
  n_electrons_per_trap_express_ov = n_electrons_per_trap;
  output( 10, "Done.\n" );
}


void   cte_image_classic::clock_charge_clear( void )
{
  // clean the traps array
  for (int i=0;i<parameters->n_levels*parameters->n_species;++i)
  {
    traps[i] = 0.0;
  }
  n_levels_traps = 0;
  stat_count = 0;
}

void   cte_image_classic::clock_charge_save_traps( void )
{

}


void   cte_image_classic::clock_charge_restore_traps( void )
{

}


double cte_image_classic::clock_charge_pixel_release( void )
{
  // Release any trapped electrons, using the appropriate decay half-life

  // release a number of electrons in the traps
  // trapped electrons relased exponentially

  double sum = 0.0;

  int    n_species = parameters->n_species;

  int    i, j;

  for (i=0;i<n_levels_traps*n_species;++i)
  {
    double release = traps[i] * exponential_factor[i];
    traps[i] -= release;
    sum += release;
  }

  // cleanup traps

  // first go through all trap levels (backwards) to eliminate the ones
  // which are almost empty
  // the loop stops at the level which sum is higher than empty_trap_limit
  // all levels above are automaticall filled with more electrons
  for (i=n_levels_traps-1;i>=0;--i)
  {
    double sum2 = 0.0;

    int    pos = i*n_species;

    for (j=0;j<n_species;++j)
    {
      sum2 += traps[pos+j];
    }
    if ( sum2 > empty_trap_limit )
    {
      break;
    }

    // clean the complete level!
    for (j=0;j<n_species;++j)
    {
      traps[pos+j] = 0.0;
    }

    n_levels_traps = i;
    stat_count++;
  }


  return sum;
}

// prepare the electron caputre by calculating the total_capture
double cte_image_classic::clock_charge_pixel_total_capture( double el_height, double i_pixelp1 )
{
  double trap_sum, total_capture;
  double  c;

  double  dheight, ov;

  int    i, j;

  int    n_species = parameters->n_species;

  // prepare the work

  // express correction using i_pixel instead of express_factor_pixel
  n_electrons_per_trap_express = n_electrons_per_trap * i_pixelp1;
  n_electrons_per_trap_express_total = n_electrons_per_trap_total * i_pixelp1;

  // calculate the height in ( affected levels )
  dheight = parameters->n_levels * el_height;

  // cheight is the last full filled trap level
  cheight = ceil( dheight ) - 1;

  // ov is the fraction of the last filled trap level
  ov = dheight - cheight;

  // some helpers
  n_electrons_per_trap_express_ov = n_electrons_per_trap_express * ov;

  // calculate the number of electrons which can be
  // captured  in the traps

  trap_sum = 0.0;
  for (i=0;i<n_species*cheight;++i)
  {
    trap_sum += traps[i];
  }

  total_capture = (n_electrons_per_trap_express_total*cheight) - trap_sum;

  for (j=0;j<n_species;++j)
  {
    int pos = (cheight*n_species)+j;
    c = n_electrons_per_trap_express[j] * ov - traps[pos];

    if ( c > 0.0 )
    {
      total_capture += c;
    }
  }

  #ifdef __debug
  std::valarray<double> pot_capture( 0.0, n_species );

  for (i=cheight+1;i<n_levels_traps;++i)
  {
    int pos = (i*n_species);
    for (j=0;j<n_species;++j)
    {
      pot_capture[j] += n_electrons_per_trap_express[j] - traps[pos+j];
    }
  }

  output( 10, "ntrap_total : %.15f\n", traps.sum() );

  print_traps();
  output( 10, "dheight     : %.15f\n", dheight );
  output( 10, "cheight,ch-1: %i %i\n", cheight+1, cheight);
  output( 10, "max_capture : %.15f\n", total_capture );
  output( 10, "pot_capture : %.15f %.15f %.15f = %.15f\n",
  pot_capture[0], pot_capture[1], pot_capture[2], pot_capture.sum() );
  #endif


  return total_capture;
}


void   cte_image_classic::clock_charge_pixel_capture_ov( double d )
{
  int    i, j;
  double c;

  int    n_species = parameters->n_species;

  // less electrons than required for capturing process
  for (i=0;i<cheight;++i)
  {
    for (j=0;j<n_species;++j)
    {
      int pos = (i*n_species)+j;

      c = ( n_electrons_per_trap_express[j] - traps[pos] ) * d;
      traps[pos] += c;
    }
  }
  for (j=0;j<n_species;++j)
  {
    int pos = cheight*n_species+j;

    if ( traps[pos] < ( n_electrons_per_trap_express_ov[j] ) )
    {
      c = ( ( n_electrons_per_trap_express_ov[j]  ) - traps[pos] ) * d;
      traps[pos] += c;
    }
  }
}


void   cte_image_classic::clock_charge_pixel_capture_full( void )
{
  int i, j;
  int pos = 0; // pos is walking trough the whole array

  // more electrons than required for capturing process
  // -> fill all traps to the max

  int    n_species = parameters->n_species;

  for (i=0;i<cheight;++i)
  {
    for (j=0;j<n_species;j++)
    {
      //pos = (i*n_species)+j;
      traps[pos] = n_electrons_per_trap_express[j];
      ++pos;
    }
  }

  for (j=0;j<n_species;++j)
  {
    if ( traps[pos] < ( n_electrons_per_trap_express_ov[j] ) )
    {
      traps[pos] = ( n_electrons_per_trap_express_ov[j] );
    }
    ++pos;
  }
}


void   cte_image_classic::clock_charge_pixel_cleanup( void )
{
  if ( (cheight+1) > n_levels_traps )
  {
    n_levels_traps = cheight+1;
  }

  #ifdef __debug
  print_traps();
  #endif
}


double cte_image_classic::clock_charge_trap_info( void )
{
  double total = 0.0;

  for (int i=0;i<n_levels_traps*parameters->n_species;++i)
  {
    total += traps[i];
  }

  return total;
}
