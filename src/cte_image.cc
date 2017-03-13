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

/* cte_image.cc

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2017-03-13


   $Id$

*/

#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iomanip>

#include "cte_image.hh"
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



void minmax_limit_array( std::valarray<long> & xrange, long min, long max )
{
  if (  xrange.size() >= 2 )
  {
    if ( xrange[0] < min ) xrange[0] = min;
    if ( xrange[1] > max ) xrange[1] = max;
  }
}

void minmax_limit( double & val, double min, double max )
{
  if ( val < min )
    val = min;
  else if ( val > max )
    val = max;
}




cte_image::cte_image( std::shared_ptr<params> p )
{
  parameters = p;

}


void cte_image::setup( long w, long h )
{
  // initialize variables

  n_species        = parameters->trap_density.size();
  n_levels         = parameters->n_levels;
  well_depth       = parameters->well_depth;
  well_notch_depth = parameters->well_notch_depth;
  well_fill_power  = parameters->well_fill_power;
  well_range       = well_depth - well_notch_depth;
  express          = parameters->express;
  readout_offset   = parameters->readout_offset;

  traps_total      = parameters->trap_density.sum();


  rotate = parameters->rotate;
  direction = parameters->direction;



  // print model information

  output( 1, "Model has %i trap species:\n", parameters->n_species );
  output( 1, " Nr  density  lifetime\n" );
  for (int i=0;i<parameters->n_species;i++)
    output( 1, " %2i: %f %f\n", i+1, parameters->trap_density[i], parameters->trap_lifetime[i] );


  // sets the image parameters according to the rotation paramater

  image_width = w;
  image_height = h;

  if ( rotate == image_readout_y )
  {
    width  = w;
    height = h;
  }
  else
  {
    // the image is rotated
    width = h;
    height = w;
  }


  // general setups and inits

  // new code with variable express
  express_multiplier = std::valarray<int> ( 0, express *  (height+1 ) );
  create_express_multiplier( express_multiplier,  height );

  // create the exponentia factors
  create_exponential_factor();


  // run the algorithm specific setups
  clock_charge_setup();



  // image slicer definitions
  is = std::image_slice( image_width,
                         image_height,
                         0,
                         rotate,
                         direction );

}


double cte_image::get_difftime( struct timeval start, struct timeval end )
{
  double seconds;
  double microseconds;

  seconds = end.tv_sec - start.tv_sec;
  microseconds = end.tv_usec - start.tv_usec;

  if ( microseconds < 0 )
    {
      seconds -= 1;
      microseconds = 1000000 - microseconds;
    }

  microseconds /= 1e6;

  return ( seconds + microseconds );
}


long cte_image::get_sparse_pixels( std::valarray<double> & v, double limit )
{
  long nr = 0;

  for (unsigned long i=0;i<v.size();++i)
    if ( ( v[i] > 0.0 ) && ( v[i] < limit ) )
      nr++;

  return nr;
}


void cte_image::limit_to_max( std::valarray<double> & v, double limit )
{
  for (unsigned long i=0;i<v.size();i++)
    if ( v[i] > limit )
      v[i] = limit;
}


void cte_image::create_express_multiplier( std::valarray<int> & emp,
                                           int h )
{
  output( 10, "Create express_multiplier...\n" );
  for (int i_pixel=0;i_pixel<height+1;++i_pixel)
  {
    int i_sum = 0;
    for (int i_express=0;i_express<express;i_express++)
    {
      int pos;
      int d;
      int d2;

      d = ( i_pixel + 1 + readout_offset);
      d2= ((h+1+readout_offset)*(i_express+1))/express;
      if ( d > d2 )
        d = d2;

      d -= i_sum;
      i_sum += d;

      pos = (i_express*(height+1))+i_pixel;
      emp[pos] = d;
     }
    }
    #ifdef __debug
    for (int i=0; i<height;i++)
    {
      for (int j=0;j<express;++j)
         std::cout << emp[j*(height+1)+i] << " ";
      std::cout << std::endl;
    }
    #endif
  output( 10, "Done.\n" );
}


void cte_image::create_exponential_factor( void )
{
  output( 10, "Create exponential factor ...\n" );
  exponential_factor = std::valarray<double> ( 0.0, n_species*n_levels );

  for (int i=0;i<n_levels;++i)
    for (int j=0;j<n_species;++j)
      exponential_factor[(i*n_species)+j] = 1 - exp( -1.0 / parameters->trap_lifetime[j] );
  output( 10, "Done.\n" );
}


void cte_image::clock_charge_setup( void )
{
  // nothing to do in the base class
  output( 10, "Hallo Berta\n" );
}


void cte_image::clock_charge_clear( void )
{
  // clear any information for the next run (express/column)
}


double cte_image::clock_charge_pixel_release( void )
{
  return 0.0;
}


double cte_image::clock_charge_pixel_total_capture( double,  int )
{
  return 0.0;
}


void cte_image::clock_charge_pixel_capture_ov( double )
{
  // handle a single pixel
}


void cte_image::clock_charge_pixel_capture_full( void )
{
  // handle a single pixel
}


void cte_image::clock_charge_pixel_cleanup( void )
{

}


// clock_charge_trap_info gives the amount of electrons located in the traps
double cte_image::clock_charge_trap_info( void )
{
  return 0.0;
}


// template for saving the trap structure
void cte_image::clock_charge_save_traps( void )
{
}


// template for restoring the trap structure
void cte_image::clock_charge_restore_traps( void )
{
}


void cte_image::clock_charge_column( std::valarray<double> & image,
                                     long i_column,
                                     long start_y,
                                     long end_y )
{
  double im, sum, d, freec;
  double el_height;              // height of electron cloud
  double total_capture;
  double express_correct;

  int express_factor_pixel = -1;
  int last_express_factor_pixel;
  bool traps_saved = false;

  int  i_express, i_pixel, i_pixelp1;


  // p_express_multiplier is a column pointer of the express array
  int p_express_multiplier = 0;

  for (i_express=0;i_express<express;++i_express)
    {
      // traps are empty
      clock_charge_clear();

      // good question if this is really necessary, because the array is used via nr_trapl as an indication
      // how much levels are really used, these levels have then also valid entries ...
      //trapl = std::valarray<std::valarray<double>>( std::valarray<double>(0.0, n_species), n_levels );
      //trapl_fill = std::valarray<int> ( 0, n_levels );
      is.reset( i_column ); // initialize the image slicer

      for (i_pixel=0;i_pixel<(end_y-start_y);++i_pixel)
        {

          i_pixelp1 = i_pixel + 1;

          // the low signal mode produces some error with express > 1
          // the problem is that in the express loop for express values > 1
          // the first lines have a multiplier =0 and therefor empty traps
          // which should have at least a filled situation ...

          last_express_factor_pixel = express_factor_pixel;



          // inner pixel loop

          // check if we need to calculate a new trail for that pixel

          // access the express array only once and use this value twice ;-)
          //express_factor_pixel = express_multiplier[p_express_multiplier + i_pixel];
          express_factor_pixel = express_multiplier[p_express_multiplier];


          // correcttion factor
          express_correct = (double) express_factor_pixel / i_pixelp1;

          #ifdef __debug
          output( 10, "express_correct = %f\n", express_correct );
          #endif

          // in express >1 mode save states when an exress block ends
          if ( ( last_express_factor_pixel == express_factor_pixel ) && ( traps_saved == false ) )
          {
            traps_saved = true;
            #ifdef __debug
            output( 10, "Traps will be saved!\n");
            output( 10, "pixel=%i express=%i\n", i_pixel, i_express );
            #endif

            clock_charge_save_traps();
          }


          if ( express_factor_pixel != 0 )
            {
              // restore the saved trap states
              if ( last_express_factor_pixel == 0 )
              {
                traps_saved = false;
                #ifdef __debug
                output( 10, "Traps will be restored!\n");
                output( 10, "pixel=%i express=%i\n", i_pixel, i_express );
                #endif

                clock_charge_restore_traps();
              }


              // extract pixel
              im = image[ (*is) ];

              // shape pixel value
              minmax_limit( im, 0.0, well_depth );

              #ifdef __debug
              output( 10, "debug : %i\n" , i_pixel );
              output( 10, "--------------------------------\n" );
              output( 10, "freec       : %f\n", im );
              output( 10, "................................\n" );
              #endif

              sum = clock_charge_pixel_release();
              freec = im + sum * express_correct;

              #ifdef __debug
              output( 10, "release     : %f\n", sum );
              output( 10, "express_corr: %f\n", express_correct );
              output( 10, "freec       : %f\n", freec );
              #endif

              // Capture any free electrons in the vicinity of empty traps

              if ( freec > well_notch_depth )
                {
                  el_height = pow(((freec - well_notch_depth ) / well_range ),well_fill_power);
                  minmax_limit( el_height , 0.0, 1.0 );

                  // calculate the total_capture
                  total_capture = clock_charge_pixel_total_capture( el_height, i_pixelp1 );

                  // correct the total_capture for express
                  total_capture *= express_correct;

                  // limit the total_capture
                  if ( total_capture < 1e-14 )
                    total_capture = 1e-14;

                  //  d gives a hint of how much electrons are available
                  // for the capturing process
                  // d > 1  more electrons are available
                  // d < 1  less electrons are available, in
                  //         the end d * total_capture is then used!
                  d = freec / total_capture;
                  if ( d < 1.0 )
                  {
                    #ifdef __debug
                    output( 10, "d < 1.0\n" );
                    #endif

                    clock_charge_pixel_capture_ov( d );
                  }
                  else
                  {
                    #ifdef __debug
                    output( 10, "d >= 1.0\n" );
                    #endif

                    clock_charge_pixel_capture_full();
                  }

                  // remove the captured electrons
                  freec -= total_capture;

                  // cleanup trap structure
                  clock_charge_pixel_cleanup();
                }

              #ifdef __debug
              output( 1, "--------------------------------\n" );
              #endif


              #ifdef __debug
              double trail = ( freec - im );
              output( 10, "strail: %.10f\n", trail );
              #endif

              image[(*is)] += ( freec - im );;


            } /* end of p_express_multiplier[i_pixel] != 0 */
            // nevertheless that we are doing nothing, change the slicer
            // next image element
            ++p_express_multiplier;
            ++is;
         } /* i_pixel loop for the trail */
      //p_express_multiplier += height + 1;
      ++p_express_multiplier;
    }  /* end of express loop */
}


// perfoming the CTI correction with clock_change_image

void cte_image::clock_charge_image( std::valarray<double> & image,
				    std::valarray<long> & xrange,
				    std::valarray<long> & yrange )
{
  // local variables copied from parameter set
  // makes the code faster, since the access of the class pointer is
  // only once ...

  /* CTE variables */
  int     sparse_pixels;

  int     start_x, start_y, end_x, end_y;
  int     i_column;


  /* time measurement */
  struct timeval start_time;
  struct timeval temp_time;
  double  diff_time;
  double  eta_time;

  struct rusage  cpu_start_time;
  struct rusage  cpu_temp_time;
  double  cpu_diff_time;

  // copy dimension parameters
  if ( xrange.size() <  2 )
    {
      start_x = 0;
      end_x = width;
    }
  else
    {
      start_x = xrange[0];
      end_x = xrange[1];
    }

  if ( yrange.size() < 2 )
    {
      start_y = 0;
      end_y = height;
    }
  else
    {
      start_y = yrange[0];
      end_y = yrange[1];
    }

  output( 10, "start_x=%i end_x=%i\n", start_x, end_x );
  output( 10, "start_y=%i end_y=%i\n", start_y, end_y );


  // info about the image and algorithm
  sparse_pixels = get_sparse_pixels( image, traps_total );

  output( 1, "There are %i pixels containing more traps than charge.\n\
The order in which these traps should be filled is ambiguous.\n", sparse_pixels );

  output( 1, "Using Jay Anderson's trick to speed up runtime\n" );



  // initialize the time measurement
  gettimeofday( &start_time, NULL );
  getrusage( RUSAGE_SELF, &cpu_start_time );

  // start the CTI correction loops

  for (i_column=start_x;i_column<end_x;++i_column)
    {
      clock_charge_column( image, i_column, start_y, end_y );

      //#ifdef __debug
      //output( 1, "\n" );
      //#endif

      /* time measurement */
      if ( ( i_column % 200 ) == 0 )
        {
          gettimeofday( &temp_time, NULL );
          getrusage( RUSAGE_SELF, &cpu_temp_time);
          diff_time = get_difftime( start_time, temp_time );
	        cpu_diff_time = get_difftime( cpu_start_time.ru_utime, cpu_temp_time.ru_utime );
          eta_time = ( diff_time / ( (i_column+1) - start_x ) ) * ( end_x - start_x );

          output( 1, "Clocking column #%i/%i in %fs, or %fs/column. ETA %.1fs.\n",
                  i_column+1, ( end_x - start_x ),
                  cpu_diff_time,
                  ( cpu_diff_time / ( ( i_column + 1 ) - start_x ) ),
                  ( eta_time - diff_time ) );

        }

      //output( 1, "stat_count=%i\n", stat_count );


	    traps_total = clock_charge_trap_info();
	    output( 11, "%5i trap fill end: %f\n", i_column, traps_total );

    } /* end of i_column loop */

  // final time measurement
  gettimeofday( &temp_time, NULL );
  getrusage( RUSAGE_SELF, &cpu_temp_time);
  diff_time = get_difftime( start_time, temp_time );
  cpu_diff_time = get_difftime( cpu_start_time.ru_utime, cpu_temp_time.ru_utime );
  output( 1, "Clocking #%i finished  in %fs REALTIME (CPU: %.2f%%)\n",
          (end_x - start_x), diff_time, (( cpu_diff_time / diff_time ) * 100.0 ) );
  output( 1, "                          %fs CPU TIME, or %fs/column\n",
          cpu_diff_time, ( cpu_diff_time  / ( end_x - start_x ) ) );

  // free local variables
  // free( express_multiplier );

  output( 10, "Done.\n" );
}




void cte_image::clock_charge( std::shared_ptr<std::valarray<double>> im,
			      std::valarray<long> & xrange,
			      std::valarray<long> & yrange )
{
  output( 11, "cte_image::clock_charge\n" );



  // make a local copy of the image data
  std::valarray<double> image( (*im) );

  std::valarray<double> trail( 0.0, image.size() );


  // check and liit the range variable
  minmax_limit_array( xrange, 0, width );
  minmax_limit_array( yrange, 0, height );


  if ( parameters->unclock )
    {
      // unclocking mode
      for (unsigned int iteration=0;iteration<parameters->n_iterations;iteration++)
	       {
	          output( 1, "Iteration %i/%i\n", iteration+1, parameters->n_iterations );

	          //if ( have_negative_values( model, naxes[0], naxes[1] ) == 1 )
            //  output( 1, "Image contains negative pixels!\n" );

            //model_readout = copy_image( (char*)model, naxes[0], naxes[1], DOUBLE_IMG );

	          // image holds now the original image + trail

	          trail = image;

            clock_charge_image( trail, xrange, yrange );


	          // do some statistics after work
            output( 1, "Minmax(old image): %f %f\n", image.min(), image.max() );

            // sub the trail
	          image -= trail;
	          image += (*im);

	          //for (unsigned int i=0;i<image.size();i++)
	          //  trail[i] = 0.0;

	          // limit_to_max
            //limit_to_max( image, (65536.-3) );

	          if ( parameters->cut_upper_limit == true )
	             limit_to_max( image, parameters->upper_limit );

            output( 1, "Minmax(new image): %f %f\n", image.min(), image.max() );

	         }
    }
  else
    {
      // clocking mode

      // no iteration is necessary , the calculated CTI is correct

      // do something
      clock_charge_image( trail, xrange, yrange );

      output( 1, "Minmax(old image): %f %f\n", (*im).min(), (*im).max() );
      output( 1, "Minmax(new image): %f %f\n", image.min(), image.max() );
    }

  // copy the data back into the original image
  (*im) = image;
  //(*im) = (*trail);
}
