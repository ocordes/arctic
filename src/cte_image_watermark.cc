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

/* cte_image_watermark.cc

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2017-06-21


   $Id$

*/

#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iomanip>

#include "cte_image.hh"
#include "cte_image_watermark.hh"
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


cte_image_watermark::cte_image_watermark( void )
                                        : cte_image( )
{
  // initialize member variables
  check_empty_traps          = false;
  empty_trap_limit           = 1e-14;
  empty_trap_limit_neo2      = 1e-14;
  neo2_split_limit           = 1e-10;

  dark_mode                  = false;

  nr_wml                     = 0;
  new_nr_wml                 = 0;
  saved_nr_wml               = 0;

  trap_density_total         = 0.0;
  trap_density_express_total = 0.0;

  cloud_height               = 0.0;
}


cte_image_watermark::cte_image_watermark( std::shared_ptr<params> & p )
                                        : cte_image( p )
{
  // initialize member variables
  check_empty_traps          = false;
  empty_trap_limit           = 1e-14;
  empty_trap_limit_neo2      = 1e-14;
  neo2_split_limit           = 1e-10;

  dark_mode                  = false;

  nr_wml                     = 0;
  new_nr_wml                 = 0;
  saved_nr_wml               = 0;

  trap_density_total         = 0.0;
  trap_density_express_total = 0.0;

  cloud_height               = 0.0;
}


#ifdef __debug
void cte_image_watermark::print_wml( void )
{
  output( 10, "trap array output (n_species=%i, trap_levels=%i):\n",
          parameters->n_species, nr_wml );
  double k = 0.0;
  for (unsigned int j=nr_wml;j>0;--j)
  {
    std::string s = "";
    for (unsigned int i=0;i<parameters->n_species;i++)
    {
      std::stringstream str;
      str << std::fixed << std::setprecision( debug_precision ) << wml[j-1][i] << " ";
      s += str.str();
    }

    std::stringstream str;
    str << std::fixed << std::setprecision( debug_precision ) << wml[j-1].sum();
    s += "= " + str.str();
    //s += "= " + std::to_string( trapl[j].sum()  );

    //output( 10, "%05i: %s\n", j, s.c_str() );
    output( 10, "%011.5f: %s  (x %f)\n", k * (double) parameters->n_levels, s.c_str(), wml_fill[j-1] );

    k += wml_fill[j-1];
  }
}
#endif


void   cte_image_watermark::clock_charge_setup( void )
{
  // warning for the highly experimental code
  output( 10, "===========================================================================\n" );
  output( 10, "= WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! =\n" );
  output( 10, "=                                                                         =\n" );
  output( 10, "= The neo2 algorithm is highly experimental! Please use this code only    =\n" );
  output( 10, "= testing purposes. The results may changed during the different          =\n" );
  output( 10, "= versions.                                                               =\n" );
  output( 10, "=                                                                         =\n" );
  output( 10, "= Feedback is wanted! Please contact the authors!                         =\n" );
  output( 10, "=                                                                         =\n" );
  output( 10, "= WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! =\n" );
  output( 10, "===========================================================================\n" );

  output( 1, "Using Olli's watermark algorithm!\n" );

  check_empty_traps     = parameters->check_empty_traps;
  empty_trap_limit      = parameters->empty_trap_limit;
  empty_trap_limit_neo2 = parameters->empty_trap_limit_neo2;
  neo2_split_limit      = parameters->neo2_split_limit;

  dark_mode = parameters->dark_mode;


  if ( dark_mode )
  {
    output( 1, " Using Dark_mode optimization!\n" );
  }

  output( 10, "Create trap structure ...\n" );

  // the maximun entries in the trap level arraay should be 2x the amount of
  // pixels in the working column, because in the worst case 2 new entries
  // will be created per pixel!

  unsigned int max_wm_levels = height * 2;

  trap_density = parameters->trap_density;
  trap_density_total          = trap_density.sum();

  // trap level information implementations
  wml = std::valarray<std::valarray<double>>( std::valarray<double>(0.0, parameters->n_species), max_wm_levels );
  wml_fill = std::valarray<double> ( 0.0, max_wm_levels );
  nr_wml = 0;

  // helper array for the d < 0.0 mode
  new_wml = std::valarray<std::valarray<double>>( std::valarray<double>(0.0, parameters->n_species), max_wm_levels );
  new_wml_fill = std::valarray<double> ( 0.0, max_wm_levels );
  new_nr_wml = 0;

  // helper array for trap saving
  saved_wml = std::valarray<std::valarray<double>> ( std::valarray<double>(0.0, parameters->n_species), max_wm_levels );
  saved_wml_fill = std::valarray<double> ( 0.0, max_wm_levels );
  saved_nr_wml = 0;

  trap_density_express = trap_density;
  trap_density_express_total = trap_density_total;

  output( 10, "Done.\n" );
}


void   cte_image_watermark::clock_charge_clear( void )
{
  // traps are empty
  nr_wml   = 0;
}


void   cte_image_watermark::clock_charge_save_traps( void )
{
  for (unsigned int i=0;i<nr_wml;++i)
  {
    saved_wml[i]      = wml[i];
    saved_wml_fill[i] = wml_fill[i];
  }
  saved_nr_wml = nr_wml;
}


void   cte_image_watermark::clock_charge_restore_traps( void )
{
  for (unsigned int i=0;i<saved_nr_wml;++i)
  {
    wml[i] = saved_wml[i];
    wml_fill[i] = saved_wml_fill[i];
  }

  nr_wml = saved_nr_wml;
}


double cte_image_watermark::clock_charge_pixel_release( void )
{
  double sum = 0.0;
  // trapped electrons relased exponentially
  for (unsigned int j=0;j<nr_wml;++j)
  {
    double sum2 = 0.0;
    for (unsigned int i=0;i<n_species;++i)
    {
      double release = wml[j][i] * exponential_factor[i];
      wml[j][i] -= release;
      sum2 += release;
    }
    // do the multiplication at the end ;-)
    sum += sum2 * wml_fill[j];
  }

  return sum;
}


double cte_image_watermark::clock_charge_pixel_total_capture( double el_height, double i_pixelp1 )
{
  double total_capture = 0.0;


  // express correction using i_pixel instead of express_factor_pixel
  trap_density_express = trap_density * i_pixelp1;
  trap_density_express_total = trap_density_total * i_pixelp1;

  cloud_height = el_height;

  // calculate the number of electrons which can be
  // captured  in the traps

  // scan all levels
  double h = 0.0;
  for (int j=nr_wml-1;j>=0;--j)
  {
    double h2 = h + wml_fill[j];

    // don't need to check for max. because
    // n_electrons_per_trap_express is always higher or
    // equal then the last step -> structure of the
    // express array, and the trap cannot hold more
    // electrons then n_electrons_per_trap_express of the

    // last step!
    if ( h2 < cloud_height )
    {
      // this levels are going directly into the calculations
      total_capture += ( trap_density_express_total - wml[j].sum() ) * wml_fill[j];
    }
    else
    {
      total_capture += ( trap_density_express_total - wml[j].sum() ) * ( cloud_height - h );
    }
    h = h2;
    if ( h > cloud_height )
    {
      break;
    }
  }

  // h has the height of all used levels
  if ( h < cloud_height )
  {
    total_capture +=  trap_density_express_total * ( cloud_height - h );
  }

  #ifdef __debug
  double traps_total2 = 0.0;
  for (unsigned int i=0;i<nr_wml;++i)
  {
    traps_total2 += wml[i].sum() * wml_fill[i];
  }
  output( 10, "ntrap_total : %.15f\n", traps_total2 );

  print_wml( wml, wml_fill, n_species, nr_wml );

  output( 10, "dehight     : %.15f\n", freec, cloud_height );
  output( 10, "max_capture : %.15f\n", total_capture );

  #endif

  return total_capture;
}

void cte_image_watermark::clock_charge_pixel_capture_ov_copyback_temp( void )
{
  #ifdef __debug
  output( 10, "Copying back the temporary data ..\n" );
  #endif

  // copy the temporary array back
  unsigned int i, j;
  for (i=0, j=new_nr_wml-1; i<new_nr_wml; ++i, --j)
  {
    wml[i]      = new_wml[j];
    wml_fill[i] = new_wml_fill[j];
  }
  nr_wml = new_nr_wml;
}


void   cte_image_watermark::clock_charge_pixel_capture_ov( double d )
{
  double cloud_height2 = cloud_height;
  double test_height;
  double h;


  new_nr_wml = 0;


  // walk through all entries
  h = 0.0;
  for (int j=nr_wml-1;j>=0;--j)
  {
    if ( cloud_height2 < 0.0 )
    {
      // nothing is needed anymore
      // just copy that level
      new_wml[new_nr_wml] = wml[j];
      new_wml_fill[new_nr_wml] = wml_fill[j];
      ++new_nr_wml;
    }
    else
    {
      // we need to modify existing levels
      // scan each bunch of levels
      if ( ( wml_fill[j] < cloud_height2 )
            || ( fabs( wml_fill[j] - cloud_height2 ) < 1e-14 ) )
      {
        // use the whole level
        new_wml[new_nr_wml] = wml[j] + ( trap_density_express - wml[j] ) * d;
        new_wml_fill[new_nr_wml] = wml_fill[j];
        ++new_nr_wml;
      }
      else
      {
        // the complex sitiuation
        // the whole trap level needs to be split in 2 parts
        // the first half filled level
        // the untouched part
        // split only if both parts are larger than
        // neo2_split_limit!

        #ifdef __debug
        output( 10, "dheight2,trapl_fill[j],h: %.15f %.15f %.15f\n",
        cloud_height2, wml_fill[j], h );
        #endif

        // idea, just judge, what to do with the number of available levels

        if ( cloud_height2 > neo2_split_limit )
        {
          new_wml[new_nr_wml] = wml[j] + ( trap_density_express - wml[j] ) * d;
          new_wml_fill[new_nr_wml] = cloud_height2;
          ++new_nr_wml;
        }
        test_height = wml_fill[j] - cloud_height2;
        if ( test_height > neo2_split_limit )
        {
          new_wml[new_nr_wml] = wml[j];
          new_wml_fill[new_nr_wml] = test_height;
          ++new_nr_wml;
        }
      }
    } // end else if ( cloud_height2 < 0.0 )

    h += wml_fill[j];
    cloud_height2 -= wml_fill[j];
  } // end of for (j=nr_wml-1;j>=0;--j)

  // in the case we have some trap height to fill
  // if ( cloud_height2 > 0.0 )
  if ( cloud_height2 > neo2_split_limit )
  {
    // we need a level
    #ifdef __debug
    output( 10, "new level needed: dheight2 = %.15f\n", cloud_height2 );
    #endif

    new_wml[new_nr_wml] = trap_density_express * d;
    new_wml_fill[new_nr_wml] = cloud_height2;
    ++new_nr_wml;
  }

  clock_charge_pixel_capture_ov_copyback_temp();
}


void   cte_image_watermark::clock_charge_pixel_capture_full( void )
{
  double skip, h;
  double test_height;

  // cloud_height gives the number of new filling levels
  //
  // wml_fill gives the number of filled levels


  // prepare the stack for refilling

  // remove the levels which will be completely absorbed
  // with the new filling
  skip = 0.0;
  h = 0.0;

  while ( nr_wml != 0 )
  {
    h += wml_fill[nr_wml-1];
    if ( h > cloud_height )
    {
      break;
    }
    else
    {
      skip = h;
    }
    --nr_wml;
  }
  // skip now containes the number of previous filled levels which
  // will completely absorbed with the new filling, the
  // corresponding levels are removed from the stack


  #ifdef __debug
  output( 10, "h,nr_trapl,d: %f %i %.15f\n", h, nr_wml, cloud_height );
  #endif

  if ( std::abs(skip - cloud_height) < 1e-14 )
  {
    // best solution ... but only reached, if dheight is n_levels,
    // which means a bright star or strong cosmic passed ...

    // but not correct implemented ...
    #ifdef __debug
    output( 10, "skip=%f dheight=%f\n", skip, cloud_height );
    #endif

    wml[nr_wml] = trap_density_express;
    wml_fill[nr_wml] = cloud_height;
    ++nr_wml;
  }
  else
  {
    // normal case

    // we have removed all levels from the stack which will be
    // absorbed completely during the new filling

    // skip is the number of levels which are already absorbed!

    // we are lucky, the levels are empty because we are at the
    // start or the new dheight is larger than all previous levels
    if ( nr_wml == 0 )
    {
      // an additional trap level
      wml[0] = trap_density_express;
      wml_fill[0] = cloud_height;
      nr_wml = 1;
    }
    else
    {
      #ifdef __debug
      output( 10, "tr_f[0],ch,skip: %f %f %f\n", wml_fill[0], cloud_height, skip );
      //print_wml( wml, wml_fill, n_species, nr_wml );
      #endif

      // skip levels are already absorbed

      // modify the first levels
      // using cheight because this is the number of remaining levels
      // from the prior filled trap levels

      // cheight - skip are the (sub)-levels  which needs to be absorbed
      // by the next big entry

      test_height = cloud_height - skip;
      if ( test_height < neo2_split_limit )
      {
        --nr_wml;
      }
      else
      {
        wml_fill[nr_wml-1] -= cloud_height - skip;   // absorbed levels
      }

      // check what to do anyway, if there is a chance to modify the
      // big level!

      wml[nr_wml] = trap_density_express;
      wml_fill[nr_wml] = cloud_height;
      ++nr_wml;

    } // else from if( nr_wml == 0 )
  } // else from if ( std::abs(skip - cloud_height) < 1e-14 )
}


void   cte_image_watermark::clock_charge_pixel_cleanup( void )
{
  // cleanup the array
  // removing duplicate levels ...

  if ( dark_mode )
  {
    // here is much mork to do ... OC
    unsigned int j = 0;
    for ( unsigned int i=1;i<nr_wml;++i)
    {
      #ifdef __debug
      output( 10, "i,j: %i %i\n", i, j );
      output( 10, "trapl[i,j] : %f %f\n", wml[i].sum(), wml[j].sum() );
      output( 10, "diff       : %f\n", std::abs(  wml[i].sum() - wml[j].sum() ) );
      #endif

      if ( std::abs( wml[i].sum() - wml[j].sum() ) < empty_trap_limit_neo2 )
      {
        double cc = wml_fill[j] + wml_fill[i];
        wml[j] *= wml_fill[j];
        wml[j] += wml[i] * wml_fill[i];
        wml[j] /= cc;
        wml_fill[j] += wml_fill[i];
      }
      else
      {
        ++j;
        if ( i != j )
        {
          wml[j] = wml[i];
          wml_fill[j] = wml_fill[i];
        }
      }
    } // end for (i=1;i<nr_wml;++i)
    nr_wml = j + 1;
  }

                //   // cleanup the array
                //   // - removing/merge duplicated levels ...
                //   // maybe it is better to stop the scanning after the first valid level?
                //   if ( dark_mode  == 1 )
                //         {
                //        j = 0;
                //        for (i=1;i<nr_trapl;++i)
                //          {
// #ifdef __debug
//             output( 10, "i,j: %i %i\n", i, j );
//             output( 10, "trapl[i,j] : %f %f\n", trapl[i].sum(), trapl[j].sum() );
//             output( 10, "diff       : %f\n", std::abs(  trapl[i].sum() - trapl[j].sum() ) );
                //             #endif
                //
                //             if ( std::abs( trapl[i].sum() - trapl[j].sum() ) < empty_trap_limit  )
                //                           {
                //                              int cc = trapl_fill[j] + trapl_fill[i];
                //                              trapl[j] *= trapl_fill[j];
                //                              trapl[j] += trapl[i] * (double) trapl_fill[i];
                //                              trapl[j] /= cc;
                //                              trapl_fill[j] += trapl_fill[i];
                //                         }
                //                   else
                //                     {
                //                        ++j;
                //                        if ( i != j )
                //                          {
                //                             trapl[j] = trapl[i];
                //                             trapl_fill[j] = trapl_fill[i];
                //                          }
                //                     }
                //          }
                //        nr_trapl = j + 1;
                //     }

                //  #ifdef __debug
                //  print_wml( wml, wml_fill, n_species, nr_wml );
                //  #endif
                // } ? maybe wrong time


}


double cte_image_watermark::clock_charge_trap_info( void )
{
  double total = 0.0;

  for (unsigned int i=0;i<nr_wml;++i)
  {
     total += wml[i].sum() * wml_fill[i];
  }

  return total;
}
