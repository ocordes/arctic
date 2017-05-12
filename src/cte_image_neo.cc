/* (C) Copyright 2013,2014,205,2016 by Oliver Cordes
        - ocordes ( at ) astro ( dot ) uni-bonn ( dot ) de

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

/* cte_image_neo.cc

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2017-05-12


   $Id$

*/

#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iomanip>

#include "cte_image.hh"
#include "cte_image_neo.hh"
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


// define some macros for the neo code

#define newtrap( new_h, new_val) ( trapl_fill[nr_trapl] = new_h; trapl[nr_trapl] = new_val; ++nr_trapl; )

// endof macros


cte_image_neo::cte_image_neo( void )
                            : cte_image( )
{
}


cte_image_neo::cte_image_neo( std::shared_ptr<params> p )
                            : cte_image( p  )
{
}

#ifdef __debug
void cte_image_neo::print_trapl( void )
{
  int i, j, k;

  std::string s;

  output( 10, "trap array output (n_species=%i, trap_levels=%i):\n",
          parameters->n_species, nr_trapl );
  k = 0;
  //for (j=0;j<nr_trapl;j++)
  for (j=nr_trapl-1;j>=0;--j)
  {
    s = "";
    for (i=0;i<parameters->n_species;++i)
    {
      std::stringstream str;
      str << std::fixed << std::setprecision( debug_precision ) << trapl[j][i] << " ";
      s += str.str();
      //s += std::to_string( trapl[j][i] ) + " ";
    }

    std::stringstream str;
    str << std::fixed << std::setprecision( debug_precision ) << trapl[j].sum();
    s += "= " + str.str();
    //s += "= " + std::to_string( trapl[j].sum()  );

    //output( 10, "%05i: %s\n", j, s.c_str() );
    output( 10, "%05i: %s  (x %05i)\n", k, s.c_str(), trapl_fill[j] );

    k += trapl_fill[j];
  }
}
#endif


bool cte_image_neo::val_array_smaller( std::valarray<double> & v1, std::valarray<double> & v2 )
{
  for ( unsigned int i=0; i<v1.size();++i)
  {
    if ( v1[i] < v2[i] )
    {
      return true;
    }
  }
  return false;
}


void cte_image_neo::clock_charge_setup( void  )
{
  output( 1, "Using Olli's neo algorithm!\n" );

  if ( dark_mode )
  {
    output( 1, " Using Dark_mode optimization!\n" );
  }


  output( 1, "Using n_levels=%i\n", parameters->n_levels );

  // setup variables
  check_empty_traps = parameters->check_empty_traps;
  empty_trap_limit = parameters->empty_trap_limit;
  empty_trap_limit_neo2 = parameters->empty_trap_limit_neo2;

  dark_mode        = parameters->dark_mode;

  n_electrons_per_trap = std::valarray<double> ( parameters->trap_density / (double) parameters->n_levels );
  n_electrons_per_trap_total = traps_total / parameters->n_levels;
  output( 10, "Create trap structure ...\n" );

  // the maximun entries in the trap level arraay should be 2x the amount of
  // pixels in the working column, because in the worst case 2 new entries
  // will be created per pixel!

  int max_trap_levels = height * 2;

  // trap level information implementations
  trapl = std::valarray<std::valarray<double>> ( std::valarray<double>(0.0, parameters->n_species), max_trap_levels );
  trapl_fill = std::valarray<int> ( 0, max_trap_levels );
  nr_trapl = 0;

  // helper array for the d < 0.0 mode
  //std::valarray<std::valarray<double>> new_trapl( std::valarray<double>(0.0, n_species), max_trap_levels );
  //std::valarray<int> new_trapl_fill( 0, max_trap_levels );
  new_trapl = trapl;
  new_trapl_fill = trapl_fill;
  new_nr_trapl = 0;

  n_electrons_per_trap_express = n_electrons_per_trap;
  n_electrons_per_trap_express_ov = n_electrons_per_trap;

  //std::valarray<std::valarray<double>> saved_trapl( std::valarray<double>(0.0, n_species), max_trap_levels );
  //std::valarray<int> saved_trapl_fill( 0, max_trap_levels );
  saved_trapl = trapl;
  saved_trapl_fill = trapl_fill;
  saved_nr_trapl = 0;

  output( 10, "Done.\n" );
}


void cte_image_neo::clock_charge_clear( void )
{
  // cöear the trap information
  nr_trapl = 0;
}


void cte_image_neo::clock_charge_save_traps( void )
{
  for (unsigned int i=0;i<nr_trapl;++i)
  {
    saved_trapl[i]      = trapl[i];
    saved_trapl_fill[i] = trapl_fill[i];
  }
  saved_nr_trapl = nr_trapl;
}


void cte_image_neo::clock_charge_restore_traps( void )
{
  for (unsigned int i=0;i<saved_nr_trapl;++i)
  {
    trapl[i] = saved_trapl[i];
    trapl_fill[i] = saved_trapl_fill[i];
  }

  nr_trapl = saved_nr_trapl;
}


double cte_image_neo::clock_charge_pixel_release( void )
{
  static double sum = 0.0;
  static double release;

  int    n_species = parameters->n_species;

  // Release any trapped electrons, using the appropriate decay half-life

  // release a number of electrons in the traps
  // trapped electrons relased exponentially

  sum = 0.0;
  for (int j=0;j<nr_trapl;++j)
  {
    double sum2 = 0.0;
    for (int i=0;i<n_species;++i)
    {
      release = trapl[j][i] * exponential_factor[i];
      trapl[j][i] -= release;
      sum2 += release;
    }
    // do the multiplication at the end ;-)
    sum += sum2 * (double) trapl_fill[j];
  }

  return sum;
}

// prepare the electron caputer by calculating the total_capture
double cte_image_neo::clock_charge_pixel_total_capture( double el_height, double i_pixelp1 )
{
   double total_capture = 0.0;
   double c;

   int    i, j, h;

   // prepare the work

   // Modifications of low signal behaviour
   //n_electrons_per_trap_express = n_electrons_per_trap * (double) express_factor_pixel;
   //n_electrons_per_trap_express_total = n_electrons_per_trap_total * express_factor_pixel;

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

   //total_capture = 0.0;

   // scan all levels
   h = 0;
   for (j=nr_trapl-1;j>=0;--j)
   {
     int h2 = h + trapl_fill[j];

     // don't need to check for max. because
     // n_electrons_per_trap_express is always higher or
     // equal then the last step -> structure of the
     // express array, and the trap cannot hold more
     // electrons then n_electrons_per_trap_express of the
     // last step!
     if ( h2 < dheight )
     {
       // this levels are going directly into the calculations
       total_capture += ( n_electrons_per_trap_express_total - trapl[j].sum() )
                                                 * trapl_fill[j];
     }
     else
     {
       total_capture += ( n_electrons_per_trap_express_total - trapl[j].sum() )
                                                 * ( cheight - h );

       for (i=0;i<n_species;++i)
       {
         c = n_electrons_per_trap_express_ov[i]  - trapl[j][i];
         if ( c > 0.0 )
         {
           total_capture += c;
         }
       }
     }
     h = h2;
     if ( h > dheight )
     {
       break;
     }
   }

   // h has the height of all used levels
   if ( h < dheight )
   {
     total_capture +=  n_electrons_per_trap_express_total * ( dheight - h );
   }

   #ifdef __debug
   double traps_total2 = 0.0;
   for (i=0;i<nr_trapl;++i)
   {
     traps_total2 += trapl[i].sum() * trapl_fill[i];
   }
   output( 10, "ntrap_total : %.15f\n", traps_total2 );
   output( 10, "n_p_t_e_t   : %.15f\n", n_electrons_per_trap_express_total );

   print_trapl();


   output( 10, "dheight     : %.15f\n", dheight );
   output( 10, "cheight,ch-1: %i %i\n", cheight+1, cheight );
   output( 10, "max_capture : %.15f\n", total_capture );

   #endif

  return total_capture;
}


void cte_image_neo::clock_charge_pixel_capture_ov_modify( int j, double d )
{
  int i;

  for (i=0;i<n_species;++i)
  {
    if ( trapl[j][i] < n_electrons_per_trap_express_ov[i] )
    {
      new_trapl[new_nr_trapl][i] = trapl[j][i]
                              + ( ( n_electrons_per_trap_express_ov[i]  ) - trapl[j][i] ) * d;
    }
    else
    {
      new_trapl[new_nr_trapl][i] = trapl[j][i];
    }
  }
  new_trapl_fill[new_nr_trapl] = 1;
  ++new_nr_trapl;
}

void cte_image_neo::clock_charge_pixel_capture_ov_copyback_temp( void )
{
  int i, j;

  // copy the temporary array back
  for (j=new_nr_trapl-1, i=0;j>=0;--j, ++i)
  {
    trapl[i]      = new_trapl[j];
    trapl_fill[i] = new_trapl_fill[j];
  }
  nr_trapl = new_nr_trapl;
}

void cte_image_neo::clock_charge_pixel_capture_ov( double d )

{
  int  j;

  // d is the fraction of electrons [0,1] which is used
  // to fill the trap levels
  // dheight is the height of the electron cloud
  // cheight = ceil( dheight ) - 1

  new_nr_trapl = 0;

  j = nr_trapl-1;     /* start with the first trap element */
  #ifdef __debug
  output( 10, "trap loop\n");
  #endif
  while ( dheight > 0.0 )
  {
    #ifdef __debug
    output( 10, "dheight       : %f\n", dheight );
    output( 10, "cheight       : %i\n", cheight );
    output( 10, "trapl_fill[j] : %i\n", trapl_fill[j] );
    #endif
    if ( j < 0 )
    {
      // add a missing level
      if ( cheight > 0 )
      {
        new_trapl[new_nr_trapl] = n_electrons_per_trap_express * d;
        new_trapl_fill[new_nr_trapl] = cheight;
        ++new_nr_trapl;
      }
      new_trapl[new_nr_trapl] = n_electrons_per_trap_express_ov * d;
      new_trapl_fill[new_nr_trapl] = 1;
      ++new_nr_trapl;

      break;
    }
    if ( dheight > trapl_fill[j] )
    {
      // use the whole level
      new_trapl[new_nr_trapl] = trapl[j]
                                + ( n_electrons_per_trap_express - trapl[j] ) * d;
      new_trapl_fill[new_nr_trapl] = trapl_fill[j];
      ++new_nr_trapl;

      // correc the values
      dheight -= trapl_fill[j];
      cheight -= trapl_fill[j];
      --j;
      continue;
    }
    if ( trapl_fill[j]  == 1 )
    {
      // just one level to work on, easy
      clock_charge_pixel_capture_ov_modify( j, d );

      dheight -= trapl_fill[j];
      cheight -= trapl_fill[j];
      --j;
      continue;
    }

    // last step the split: the trapl_fill is hight than dheight

    #ifdef __debug
    //output( 10, "cheight       : %i\n", cheight );
    output( 10, "split 3 parts\n" );
    #endif
    if ( cheight > 0 )
    {
      // first part is necessary
      new_trapl[new_nr_trapl] = trapl[j]
                           + ( n_electrons_per_trap_express - trapl[j] ) * d;
      new_trapl_fill[new_nr_trapl] = cheight;
      ++new_nr_trapl;
    }

    if ( ov > 1e-14 )
    {
      // if ov == 0! then no additional level is needed
      clock_charge_pixel_capture_ov_modify( j, d );
      // for (i=0;i<n_species;++i)
      // {
      //   if ( trapl[j][i] < n_electrons_per_trap_express_ov[i] )
      //     new_trapl[new_nr_trapl][i] = trapl[j][i]
      //                             + ( ( n_electrons_per_trap_express_ov[i]  ) - trapl[j][i] ) * d;
      //   else
      //     new_trapl[new_nr_trapl][i] = trapl[j][i];
      // }
      // new_trapl_fill[new_nr_trapl] = 1;
      // ++new_nr_trapl;
    }

    trapl_fill[j] -= ceil( dheight );

    if ( trapl_fill[j] == 0 )
    {
      --j;      // fix BUG which was also in the 1st implementation
    }

    dheight = 0.0;
  }

  while ( j >= 0 )
  {
    // nothing is needed anymore
    // just copy that level
    #ifdef __debug
    output( 10, "j             : %i\n", j);
    output( 10, "trapl_fill[j] : %i\n", trapl_fill[j] );
    #endif
    new_trapl[new_nr_trapl] = trapl[j];
    new_trapl_fill[new_nr_trapl] = trapl_fill[j];
    ++new_nr_trapl;

    --j;
  }

  // copy the temporary array back
  clock_charge_pixel_capture_ov_copyback_temp();

  #ifdef __debug
  print_trapl();
  #endif

  return;

}


void cte_image_neo::clock_charge_pixel_capture_full( void )
{
  int          j, h, skip;


  // cheight full levels
  // cheight+1 partly filled levels
  //
  // trapl_fill gives the number of filled levels


  // prepare the stack for refilling

  // remove the levels which will be completely absorbed
  // with the new filling
  skip = 0;
  h = 0;
  while ( nr_trapl != 0 )
  {
    h += trapl_fill[nr_trapl-1];
    if ( h > dheight )
    {
      break;
    }
    else
    {
      skip = h;
    }
    --nr_trapl;
  }

  // skip now containes the number of previous filled levels which
  // will completely absorbed with the new filling, the
  // corresponding levels are removed from the stack


  #ifdef __debug
  output( 10, "h,nr_trapl,d: %i %i %.15f\n", h, nr_trapl, dheight );
  #endif

  if ( std::abs(skip - dheight) < 1e-14 )
  {
    // best solution ... but only reached, if dheight is n_levels,
    // which means a bright star or strong cosmic passed ...

    // but not correct implemented ...
    #ifdef __debug
    output( 10, "skip=%i dheight=%f\n", skip, dheight );
    #endif

    trapl[nr_trapl] = n_electrons_per_trap_express_ov;
    trapl_fill[nr_trapl] = 1;
    ++nr_trapl;
    trapl_fill[nr_trapl] = cheight;
    trapl[nr_trapl] = n_electrons_per_trap_express;
    ++nr_trapl;
  }
  else
  {
    // normal case

    // we have removed all levels from the stack which will be
    // absorbed completely during the new filling

    // skip is the number of levels which are already absorbed!

    // we are lucky, the levels are empty because we are at the
    // start or the new dheight is larger than all previous levels
    if ( nr_trapl == 0 )
    {
      // an additional trap level
      trapl_fill[1] = cheight;
      trapl_fill[0] = 1;
      trapl[1] = n_electrons_per_trap_express;
      trapl[0] = n_electrons_per_trap_express_ov;
      nr_trapl = 2;
    }
    else
    {
      #ifdef __debug
      output( 10, "tr_f[0],ch,skip: %i %i %i\n", trapl_fill[0], cheight, skip );
      //print_trapl( trapl, trapl_fill, n_species, nr_trapl );
      #endif

      // cheight full trap levels
      // ov is a single level with ov * n_electrons_per_trap fill

      // skip levels are already absorbed

      // modify the first levels
      // using cheight because this is the number of remaining levels
      // from the prior filled trap levels

      // cheight - skip are the (sub)-levels  which needs to be absorbed
      // by the next big entry
      trapl_fill[nr_trapl-1] -= cheight - skip;   // absorbes levels

      // check what to do anyway, if there is a chance to modify the
      // big level!

      if ( val_array_smaller( trapl[nr_trapl-1], n_electrons_per_trap_express_ov ) )
      {
        if ( trapl_fill[nr_trapl-1] > 1 )
        {
          // split levels
          --trapl_fill[nr_trapl-1];
          trapl_fill[nr_trapl] = 1;
          trapl[nr_trapl] = trapl[nr_trapl-1];
          ++nr_trapl;
        }

        // fill only the species which needs to be filled
        for (j=0;j<n_species;++j)
        {
          // modify if there are free
          if ( trapl[nr_trapl-1][j] < n_electrons_per_trap_express_ov[j] )
          {
            trapl[nr_trapl-1][j] =  n_electrons_per_trap_express_ov[j];
          }
        }
      }

      // fill the leading trap level if necessary
      if ( cheight > 0 )
      {
        trapl_fill[nr_trapl] = cheight;
        trapl[nr_trapl] = n_electrons_per_trap_express;

        ++nr_trapl;
      }
    }
  }

  #ifdef __debug
  print_trapl();
  #endif
}


void cte_image_neo::clock_charge_pixel_cleanup( void )
{
  int i, j;

  // cleanup the array
  // - removing/merge duplicated levels ...
  // maybe it is better to stop the scanning after the first valid level?
  if ( dark_mode  == 1 )
  {
    j = 0;
    for (i=1;i<nr_trapl;++i)
    {
      #ifdef __debug
      output( 10, "i,j: %i %i\n", i, j );
      output( 10, "trapl[i,j] : %f %f\n", trapl[i].sum(), trapl[j].sum() );
      output( 10, "diff       : %f\n", std::abs(  trapl[i].sum() - trapl[j].sum() ) );
      #endif

      if ( std::abs( trapl[i].sum() - trapl[j].sum() ) < empty_trap_limit  )
      {
        int cc = trapl_fill[j] + trapl_fill[i];
        trapl[j] *= trapl_fill[j];
        trapl[j] += trapl[i] * (double) trapl_fill[i];
        trapl[j] /= cc;
        trapl_fill[j] += trapl_fill[i];
      }
      else
      {
        ++j;
        if ( i != j )
        {
          trapl[j] = trapl[i];
          trapl_fill[j] = trapl_fill[i];
        }
      }
    }
    nr_trapl = j + 1;
  }

  #ifdef __debug
  print_trapl();
  #endif
}


// gives a total number of how much electrons are located in the traps
double cte_image_neo::clock_charge_trap_info( void )
{
  double total = 0.0;

  for (unsigned int i=0;i<nr_trapl;++i)
  {
    total += trapl[i].sum() * trapl_fill[i];
  }

  return total;
}
