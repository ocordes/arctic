/* (C) Copyright 2013 by Oliver Cordes
        - ocordes ( at ) astro ( dot ) uni-bonn ( dot ) de


    This file is part of arctic-project.

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

/* cte_image_euclid.cc

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2016-06-14

   $Id: cte_image_euclid.cc 975 2016-03-09 13:08:56Z ocordes $

*/

#include <cstdlib>
#include <cstring>

#include "cte_image_euclid.hh"
#include "output.hh"
#include "params.hh"


#include <iostream>
#include <memory>


#include <sys/time.h>
#include <sys/resource.h>

//#define __debug
#define debug_pixel 800

// use to activate the comparison code
//#define compariaon



cte_image_euclid::cte_image_euclid( std::shared_ptr<params> p )
{
  parameters = p;

  // initialize variables
  empty_trap_limit = parameters->empty_trap_limit;

}


double cte_image_euclid::get_difftime( struct timeval start, struct timeval end )
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


long cte_image_euclid::get_sparse_pixels( std::valarray<double> & v, double limit )
{
  long nr = 0;

  for (unsigned long i=0;i<v.size();i++)
    if ( ( v[i] > 0.0 ) && ( v[i] < limit ) )
      nr++;

  return nr;
}


void cte_image_euclid::limit_to_max( std::valarray<double> & v, double limit )
{
  for (unsigned long i=0;i<v.size();i++)
    if ( v[i] > limit )
      v[i] = limit;
}



double cte_image_euclid::get_sum_double_array( double *array, int width, int height )
{
  double sum = 0.0;

  int    totel;
  int    i;

  totel = width * height;

  for (i=0;i<totel;i++)
    sum += array[i];

  return sum;
}


void cte_image_euclid::print_traps( std::valarray<double> & t, int n_species, int trap_levels )
{
  int i, j,c;

  double d;

  std::string s;

  output( 10, "trap array output (n_species=%i, trap_levels=%i):\n", n_species, trap_levels );
  for (j=0;j<trap_levels;j++)
    {
      s = "";
      c = 0;
      d = 0.0;
      for (i=0;i<n_species;i++)
	{
	  if ( j > 0 )
	    {
	      if ( fabs( t[(j*n_species)+i] - t[((j-1)*n_species)+i] ) < 1e-14 )
		c++;
	    }
	  s += std::to_string( t[(j*n_species)+i] ) + " ";
	  d +=  t[(j*n_species)+i];

	}
      s += "= " + std::to_string( d );
      if ( c < 3 )
	output( 10, "%05i: %s\n", j, s.c_str() );
    }
}


void cte_image_euclid::print_trapl( std::valarray<std::valarray<double>> & trapl,
			     std::valarray<int> & trapl_fill,
			     int n_species,
			     int nr_trapl )
{
  int i, j, l;

  std::string s;

  output( 10, "trap array output (n_species=%i, trap_levels=%i):\n", n_species, nr_trapl );
  l = 0;
  //for (j=0;j<nr_trapl;j++)
  for (j=nr_trapl-1;j>=0;j--)
    {
      s = "";
      for (i=0;i<n_species;i++)
	s += std::to_string( trapl[j][i] ) + " ";
      s += "= " + std::to_string( trapl[j].sum()  );

      //output( 10, "%05i: %s\n", j, s.c_str() );
      output( 10, "%05i: %s  (x %05i)\n", l, s.c_str(), trapl_fill[j] );

      l += trapl_fill[j];
    }
}


bool cte_image_euclid::val_array_smaller( std::valarray<double> & v1,
				  std::valarray<double> & v2 )
{
  for( int i=0; i<v1.size();i++)
    if ( v1[i] < v2[i] )
      return true;
  return false;
}


// original code from the IDL version adopted by R.Massey

void cte_image_euclid::clock_charge_image( std::valarray<double> & image,
                                    std::valarray<double> & trail,
                                    std::valarray<long> & xrange,
                                    std::valarray<long> & yrange )
{
  // local variables copied from parameter set
  // makes the code faster, since the access of the class pointer is
  // only once ...
  int     n_species;
  int     n_levels;
  double  well_depth;
  double  well_notch_depth;
  double  well_fill_power;
  int     express;
  int     readout_offset;

  /* CTE variables */
  //double *traps;
  double  traps_total;
  int     n_levels_traps;
  //double *n_electrons_per_trap;
  std::valarray<double> n_electrons_per_trap;
  //double *n_electrons_per_trap_express;
   std::valarray<double> n_electrons_per_trap_express;
  //double  n_electrons_per_trap_total;
  double  n_electrons_per_trap_total_express;
  int    *express_multiplier;
  int    *p_express_multiplier;
  int     i_express;
  //double *exponential_factor;
  double  release;
  //double *traps;
  double  total_capture;
  double  freec;
  double  im;
  double  well_range;
  int     cheight = 0;
  double  dheight;

  int     sparse_pixels;

  int     start_x, start_y, end_x, end_y;
  int     i_column, i_pixel;


  /* helpers */
  int     i, j;
  double  d;
  double  sum;

  int     stat_count;

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
      end_x = image_width;
    }
  else
    {
      start_x = xrange[0];
      end_x = xrange[1];
    }

  if ( yrange.size() < 2 )
    {
      start_y = 0;
      end_y = image_height;
    }
  else
    {
      start_y = yrange[0];
      end_y = yrange[1];
    }

  output( 10, "start_x=%i end_x=%i\n", start_x, end_x );
  output( 10, "start_y=%i end_y=%i\n", start_y, end_y );

  // special local variabels copied from parameter set
  // use n_species as a local variable
  n_species        = parameters->trap_density.size();
  n_levels         = parameters->n_levels;
  well_depth       = parameters->well_depth;
  well_notch_depth = parameters->well_notch_depth;
  well_fill_power  = parameters->well_fill_power;
  express          = parameters->express;
  readout_offset   = parameters->readout_offset;

  //traps_total = 0;
  //for (i=0;i<n_species;i++)
  //  traps_total += trap_density[i];
  traps_total = parameters->trap_density.sum();


  // if quantize_traps set
  // n_levels = round( 2.0 / traps_total );
  // if ( n_levels < 1  )
  //   n_levels = 1;


  // change the n_levels to a dynamic variable
  // n_levels = 10000;
  // (*pn_levels) = n_levels;

  // removed in favor of a class variable
  //n_levels = (*pn_levels);

  //n_electrons_per_trap_express = (double*) malloc( sizeof( double ) * n_species );

  //n_electrons_per_trap = (double*) malloc( sizeof( double ) * n_species );
  //for (i=0;i<n_species;i++)
  //    n_electrons_per_trap[i] = parameters->trap_density[i] / n_levels;
  n_electrons_per_trap = std::valarray<double> ( parameters->trap_density / (double) n_levels );
  //n_electrons_per_trap_total = traps_total / n_levels;

  n_electrons_per_trap_express = std::valarray<double> (n_electrons_per_trap) ;

  sparse_pixels = get_sparse_pixels( image, traps_total );

  output( 1, "There are %i pixels containing more traps than charge.\nThe order in which these traps should be filled is ambiguous.\n", sparse_pixels );

  output( 1, "Using Jay Anderson's trick to speed up runtime\n" );


  output( 1, "Using n_levels=%i\n", n_levels );

  /* new code with variable express */

  output( 10, "Create express_multiplier...\n" );
  express_multiplier = (int*) malloc( express * (image_height+1 ) * sizeof( int ) );

  for (i_pixel=0;i_pixel<image_height+1;i_pixel++)
    {
      int d;
      int d2;
      int i_sum = 0;
      for (i_express=0;i_express<express;i_express++)
        {
          int pos;
          d = ( i_pixel + 1 + readout_offset);
          d2= ((image_height+1+readout_offset)*(i_express+1))/express;
          if ( d > d2 )
            d = d2;

          d -= i_sum;
          i_sum += d;

          pos = (i_express*(image_height+1))+i_pixel;
          express_multiplier[pos] = d;
        }
     }
  output( 10, "Done.\n" );


  // array structures in general ...
  // in the IDL description we have n_species x something array
  // for traps, release, capture ..
  // Since the something is different for each array it is better to have
  // a something x n_species organization in C, so that we can first loop
  // over something and then loop over n_species

  output( 10, "Create exponential factor ...\n" );

  //exponential_factor = (double*) malloc( n_species * n_levels * sizeof( double ) );
  std::valarray<double> exponential_factor( 0.0, n_species*n_levels );


  for (i=0;i<n_levels;i++)
    for (j=0;j<n_species;j++)
      exponential_factor[(i*n_species)+j] = 1 - exp( -1.0 / parameters->trap_lifetime[j] );

  output( 10, "Done.\n" );



  well_range = well_depth - well_notch_depth;


  output( 10, "Setup traps ...\n" );

  //traps = (double*) malloc( n_species * n_levels * sizeof( double ) );
  std::valarray<double> traps(0.0, n_species * n_levels );

  output( 10, "Done.\n" );


  output( 10, "Start CTE calculations ...\n" );
  // initialize the time measurement
  gettimeofday( &start_time, NULL );
  getrusage( RUSAGE_SELF, &cpu_start_time );


  for (i_column=start_x;i_column<end_x;i_column++)
    {

      // use the multiplier as a flat array with the "pixel multiplicators"
      // in a line
      p_express_multiplier = express_multiplier;
      for (i_express=0;i_express<express;i_express++)
        {

          // clean the traps array
          for (i=0;i<n_levels*n_species;i++)
            traps[i] = 0.0;

          // traps are empty
          n_levels_traps = 0;

          stat_count = 0;


          for (i_pixel=0;i_pixel<(end_y-start_y);i_pixel++)
            {

              // inner pixel loop

              // check if we need to calculate a new trail for that pixel
              if ( p_express_multiplier[i_pixel] != 0 )
                {
                  // calculate the corrected n_electrons_per_trap for express
		  n_electrons_per_trap_express = n_electrons_per_trap * (double) p_express_multiplier[i_pixel];
                  n_electrons_per_trap_total_express = n_electrons_per_trap_express.sum();

                  // extract pixel
                  im = image[((i_pixel+start_y)*image_width)+i_column];
                  if ( im > well_depth )
                    im = well_depth;
                  if ( im < 0.0 )
                    im = 0.0;
                  freec = im;


                  // Release any trapped electrons, using the appropriate decay half-life

                  // release a number of electrons in the traps
                  // trapped electrons relased exponentially

                  sum = 0.0;
                  for (i=0;i<n_levels_traps*n_species;i++)
                    {
                      release = traps[i] * exponential_factor[i];
                      traps[i] -= release;
                      sum += release;
                    }

                  freec += sum;


                  // clear n_levels_traps

                  // this algorithm looks for empty traps and reduces
                  // the values of n_levels_traps to speedup the code,
                  // the trap array is analyzed from the prosition
                  // n_levels_traps which hold always the max position
                  // of a trap which was used before ! */

                  for (i=n_levels_traps-1;i>=0;i--)
                    {
                      int to_break = 0;

                      for (j=0;j<n_species;j++)
                        {
                          int pos = (i*n_species)+j;

                          if ( traps[pos] > empty_trap_limit )
			    {
                              to_break = 1;
                              break;
                            }
                          else
                            traps[pos] = 0.0;
                        }
                      if ( to_break == 1 )
                        break;
                      n_levels_traps = i;
                      stat_count++;
                    }


                  // Capture any free electrons in the vicinity of empty traps

                  if ( freec > well_notch_depth )
                    {
                      d = pow(((freec - well_notch_depth ) / well_range ),well_fill_power);
                      if ( d > 1.0 )
                        d =  1.0;
                      else
                        if ( d < 0.0 )
                          d = 0.0;

                      dheight = n_levels * d;



                      cheight = ceil( dheight );



                      // calculate the number of electrons which can be captured

                      // opti
                      double trap_sum = 0.0;
		      for (i=0;i<n_species*(cheight-1);i++)
                        trap_sum += traps[i];

		      #ifdef __debug
		      output( 10, "debug:  %i\n", i_pixel );
		      double sum2 = n_electrons_per_trap_total_express * dheight - trap_sum;
		      double sum3 = trap_sum;
		      #endif

		      sum = (n_electrons_per_trap_total_express*(cheight-1))
                        //- get_sum_double_array( traps, cheight-1, n_species );
			- trap_sum;
                      d = dheight - (cheight-1);
                      for (j=0;j<n_species;j++)
                        {
                          double c = 0.0;
                          int pos = (cheight-1)*n_species+j;
			  c = n_electrons_per_trap_express[j] * d - traps[pos];
			  #ifdef __debug
			  sum2 -= traps[pos];
			  sum3 += traps[pos];
			  #endif
                          if ( c > 0.0 )
                            sum += c;
                        }
                      total_capture = sum;

		      // look which method is best and fastest
		      //output( 10, "sum,sum2,ts,tf,tc: %f %f %f %f %f\n", sum, sum2, trap_sum, n_electrons_per_trap_total_express * dheight, sum3 );


		      std::valarray<double> pot_capture( 0.0, n_species );
		      for (i=cheight;i<n_levels_traps;i++)
			for (j=0;j<n_species;j++)
			  {
			    int pos = (i*n_species)+j;
			    pot_capture[j] += n_electrons_per_trap_express[j] - traps[pos];
			  }

		      #ifdef __debug
		      output( 10, "ntrap_total : %f\n", traps.sum() );
		      print_traps( traps, n_species, n_levels_traps );
		      output( 10, "free,dheight: %f %f\n", freec, dheight );
		      output( 10, "cheight,ch-1: %i %i\n", cheight, cheight-1 );
		      output( 10, "max_capture : %f\n", total_capture );
		      output( 10, "pot_capture : %f %f %f = %f\n", pot_capture[0], pot_capture[1], pot_capture[2], pot_capture.sum() );
		      #endif

                      if ( total_capture < 1e-14 )
                        total_capture = 1e-14;


                      d = freec / total_capture;
                      double ov = dheight - (cheight-1);
                      if ( d < 1.0 )
                        {
			  #ifdef __debug
			  output( 10, "d < 1.0\n" );
			  #endif
                          // less electrons than required for capturing process
                          for (i=0;i<cheight-1;i++)
                            for (j=0;j<n_species;j++)
                              {
                                int pos = (i*n_species)+j;

                                double c = ( n_electrons_per_trap_express[j] - traps[pos] ) * d;
                                traps[pos] += c;
                              }

                          for (j=0;j<n_species;j++)
			    {
                              int pos = (cheight-1)*n_species+j;

                              if ( traps[pos] < ( n_electrons_per_trap_express[j] * ov ) )
                                {
                                  double c = ( ( n_electrons_per_trap_express[j] * ov ) - traps[pos] ) * d;
                                  traps[pos] += c;
                                }
                            }
                          total_capture *= d;
                        }
                      else
                        {
                          // more electrons than required for capturing process
                          // -> fill all traps to the max
                          for (i=0;i<cheight-1;i++)
                            for (j=0;j<n_species;j++)
                              {
                                int pos = (i*n_species)+j;
                                traps[pos] = n_electrons_per_trap_express[j];
			      }

                          for (j=0;j<n_species;j++)
                            {
                              int pos = (cheight-1)*n_species+j;
                              if ( traps[pos] < ( n_electrons_per_trap_express[j] * ov ) )
                                  traps[pos] = ( n_electrons_per_trap_express[j] * ov );
                            }
                        }


                      if ( cheight > n_levels_traps )
                        n_levels_traps = cheight;

		      #ifdef __debug
		      output( 10, "ntrap_fill  : %i\n", n_levels_traps );
		      output( 10, "ntrap_total : %f\n", traps.sum() );

		      output( 10, "n_e_p_t_e   : %f %f %f = %f\n",
			       n_electrons_per_trap_express[0],
			       n_electrons_per_trap_express[1],
			       n_electrons_per_trap_express[2], n_electrons_per_trap_total_express );


		      std::valarray<double> pot_capture2( 0.0, n_species );
		      for (i=cheight;i<n_levels_traps;i++)
			for (j=0;j<n_species;j++)
			  {
			    int pos = (i*n_species)+j;
			    pot_capture2[j] += n_electrons_per_trap_express[j] - traps[pos];
			  }
		      output( 10, "pot_capture2: %f %f %f = %f\n", pot_capture2[0], pot_capture2[1], pot_capture2[2], pot_capture2.sum() );

		      print_traps( traps, n_species, n_levels_traps+1 );
		      #endif

                      // delete the captured exlectrons from
                      //   the pixel value
                      freec -= total_capture;

                    } /* end of if ( freec > well_notch_depth ) */

                  //#ifdef __debug
                  //if ( i_pixel == debug_pixel )
                  //  output( 1, "%i ", p_express_multiplier[i_pixel] );
                  //#endif

                  /* add the trail for i_pixel */

                  //(*image_data)[((i_pixel+start_y)*image_width)+i_column] += ( freec - im );

                  // save only the trail ...
		  trail[((i_pixel+start_y)*image_width)+i_column] = ( freec - im );

                } /* end of p_express_multiplier[i_pixel] != 0 */
            } /* i_pixel loop for the trail */
          p_express_multiplier += image_height +1;
        }  /* end of express loop */

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
      output( 11, "%5i trap fill end: %f\n", i_column, traps.sum() );

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

  output( 10, "Done.\n" );

  output( 10, "Freeing variables...\n" );
  // free local variables
  //free( traps );
  //free( exponential_factor );
  free( express_multiplier );
  //free( n_electrons_per_trap_express );
  output( 10, "Done.\n" );
}


// changed code with different trap level descriptions

void cte_image_euclid::clock_charge_image_neo( std::valarray<double> & image,
				    std::valarray<double> & trail,
				    std::valarray<long> & xrange,
				    std::valarray<long> & yrange )
{
  // local variables copied from parameter set
  // makes the code faster, since the access of the class pointer is
  // only once ...
  int     n_species;
  int     n_levels;
  double  well_depth;
  double  well_notch_depth;
  double  well_fill_power;
  int     express;
  int     readout_offset;
  int     dark_mode;
  int     nr_trap_levels;

  /* CTE variables */
  double  traps_total;
  double  n_electrons_per_trap_total_express;
  int    *express_multiplier;
  int    *p_express_multiplier;
  int     i_express;
  double  release;
  double  total_capture;
  double  freec;
  double  im;
  double  well_range;
  int     cheight = 0;
  double  dheight;

  int     sparse_pixels;

  int     start_x, start_y, end_x, end_y;
  int     i_column, i_pixel;


  /* helpers */
  int     i, j;
  double  d;
  double  sum;

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
      end_x = image_width;
    }
  else
    {
      start_x = xrange[0];
      end_x = xrange[1];
    }

  if ( yrange.size() < 2 )
    {
      start_y = 0;
      end_y = image_height;
    }
  else
    {
      start_y = yrange[0];
      end_y = yrange[1];
    }

  output( 10, "start_x=%i end_x=%i\n", start_x, end_x );
  output( 10, "start_y=%i end_y=%i\n", start_y, end_y );

  // special local variabels copied from parameter set
  // use n_species as a local variable
  n_species        = parameters->trap_density.size();
  n_levels         = parameters->n_levels;
  well_depth       = parameters->well_depth;
  well_notch_depth = parameters->well_notch_depth;
  well_fill_power  = parameters->well_fill_power;
  express          = parameters->express;
  readout_offset   = parameters->readout_offset;
  dark_mode        = parameters->dark_mode;

  nr_trap_levels   = 2;


  // setup variables

  traps_total = parameters->trap_density.sum();
  std::valarray<double> n_electrons_per_trap = std::valarray<double> ( parameters->trap_density / (double) n_levels );
  std::valarray<double> n_electrons_per_trap_express = std::valarray<double> ( n_electrons_per_trap );
  well_range = well_depth - well_notch_depth;

  // new code with variable express
  express_multiplier = (int*) malloc( express * (image_height+1 ) * sizeof( int ) );

  // setup of the express array
  for (i_pixel=0;i_pixel<image_height+1;i_pixel++)
    {
      int d;
      int d2;
      int i_sum = 0;
      for (i_express=0;i_express<express;i_express++)
        {
          int pos;
          d = ( i_pixel + 1 + readout_offset);
	  d2= ((image_height+1+readout_offset)*(i_express+1))/express;
          if ( d > d2 )
            d = d2;

          d -= i_sum;
          i_sum += d;

          pos = (i_express*(image_height+1))+i_pixel;
          express_multiplier[pos] = d;
        }
     }


  std::valarray<double> exponential_factor( 0.0, n_species*n_levels );

  for (i=0;i<n_levels;i++)
    for (j=0;j<n_species;j++)
      exponential_factor[(i*n_species)+j] = 1 - exp( -1.0 / parameters->trap_lifetime[j] );

  well_range = well_depth - well_notch_depth;


  // trap level information implementations
  std::valarray<std::valarray<double>> trapl( std::valarray<double>(0.0, n_species), n_levels );
  std::valarray<int> trapl_fill( 0, n_levels );
  long nr_trapl = 0;

  // helper array for the d < 0.0 mode
  std::valarray<std::valarray<double>> new_trapl( std::valarray<double>(0.0, n_species), n_levels );
  std::valarray<int> new_trapl_fill( 0, n_levels );
  long new_nr_trapl = 0;

  //std::valarray<double> vrelease( std::valarray<double>(0.0, n_species) );

  std::valarray<double> n_electrons_per_trap_express_ov = n_electrons_per_trap_express;

  int     h, h2, skip;
  double ov;
  double  c;

  double dheight2;
  int cheight2;


  // info about the image and algorithm
  sparse_pixels = get_sparse_pixels( image, traps_total );

  output( 1, "There are %i pixels containing more traps than charge.\nThe order in which these traps should be filled is ambiguous.\n", sparse_pixels );

  output( 1, "Using Jay Anderson's trick to speed up runtime\n" );

  output( 1, "Using OllI's neo algorithm!\n" );
  if ( dark_mode )
    output( 1, " Using Dark_mode optimization!\n" );


  output( 1, "Using n_levels=%i\n", n_levels );


  // initialize the time measurement
  gettimeofday( &start_time, NULL );
  getrusage( RUSAGE_SELF, &cpu_start_time );


  for (i_column=start_x;i_column<end_x;i_column++)
    {

      // use the multiplier as a flat array with the "pixel multiplicators"
      // in a line
      p_express_multiplier = express_multiplier;
      for (i_express=0;i_express<express;i_express++)
        {
          // traps are empty
	  nr_trapl   = 0;

	  // good question if this is really necessary, because the array is used via nr_trapl as an indication
	  // how much levels are really used, these levels have then also valid entries ...
	  //trapl = std::valarray<std::valarray<double>>( std::valarray<double>(0.0, n_species), n_levels );
	  //trapl_fill = std::valarray<int> ( 0, n_levels );



          for (i_pixel=0;i_pixel<(end_y-start_y);i_pixel++)
            {

              // inner pixel loop

              // check if we need to calculate a new trail for that pixel

	      if ( p_express_multiplier[i_pixel] != 0 )
                {
                  // calculate the corrected n_electrons_per_trap for express
                  n_electrons_per_trap_express = n_electrons_per_trap *  (double) p_express_multiplier[i_pixel];
                  n_electrons_per_trap_total_express = n_electrons_per_trap_express.sum();

                  // extract pixel
                  im = image[((i_pixel+start_y)*image_width)+i_column];
                  if ( im > well_depth )
                    im = well_depth;
                  if ( im < 0.0 )
                    im = 0.0;
                  freec = im;

                  // Release any trapped electrons, using the appropriate decay half-life

                  // release a number of electrons in the traps
                  // trapped electrons relased exponentially

		  sum = 0.0;
		  for (j=0;j<nr_trapl;j++)
		    {
		      for (i=0;i<n_species;i++)
		  	{
		  	  release = trapl[j][i] * exponential_factor[i];
		  	  trapl[j][i] -= release;
		  	  sum += release * trapl_fill[j];
		  	}
		    }

		  // valarray code is slower!
		  // sum = 0.0;
		  // for (j=0;j<nr_trapl;j++)
		  //   {
		  //     vrelease = trapl[j] * exponential_factor;
		  //     trapl[j] -= vrelease;
		  //     sum += vrelease.sum() * trapl_fill[j];
		  //   }



                  freec += sum;


                  // Capture any free electrons in the vicinity of empty traps

                  if ( freec > well_notch_depth )
                    {
                      d = pow(((freec - well_notch_depth ) / well_range ),well_fill_power);
                      if ( d > 1.0 )
                        d =  1.0;
                      else
                        if ( d < 0.0 )
                          d = 0.0;

		      dheight = n_levels * d;



		      // cheight is the last full filled trap level
                      cheight = ceil( dheight ) - 1;

		      // ov is the fraction of the last filled trap level
		      ov = dheight - cheight;



                      // calculate the number of electrons which can be captured


		      // new code

		      total_capture = 0.0;

		      // scan all levels
		      h = 0;
		      for (j=nr_trapl-1;j>=0;j--)
			{
			  h2 = h + trapl_fill[j];


			  // don't need to check for max. because n_electrons_per_trap_express is
			  // always higher or equal then the last step -> structure of the express
			  // array, and the trap cannot hold more electrons then n_electrons_per_trap_express
			  // of the last step!
			  if ( h2 < dheight )
			    {
			      // this levels are going directly into the calculations
			      total_capture += ( n_electrons_per_trap_total_express - trapl[j].sum() ) * trapl_fill[j];
			    }
			  else
			    {
			      total_capture += ( n_electrons_per_trap_total_express - trapl[j].sum() ) * ( cheight - h );

			      for (i=0;i<n_species;i++)
				{
				  c = n_electrons_per_trap_express[i] * ov - trapl[j][i];
				  if ( c > 0.0 )
				    total_capture += c;
				}
			    }
			  h = h2;
			  if ( h > dheight )
			    break;
			}

		      // h has the height of all used levels
		      if ( h < dheight )
			{
			  total_capture +=  n_electrons_per_trap_total_express * ( dheight - h );
			}

		      #ifdef __debug
		      output( 10, "debug:  %i\n", i_pixel );
		      double traps_total = 0.0;
		      for (i=0;i<nr_trapl;i++)
			traps_total += trapl[i].sum() * trapl_fill[i];
		      output( 10, "ntrap_total : %f\n", traps_total );

		      print_trapl( trapl, trapl_fill, n_species, nr_trapl );


		      output( 10, "free,dheight: %f %f\n", freec, dheight );
                      output( 10, "cheight,ch-1: %i %i\n", cheight+1, cheight );
                      output( 10, "max_capture : %f\n", total_capture );

                      #endif

                      if ( total_capture < 1e-14 )
                        total_capture = 1e-14;

		      // use cheight -1 from this point

                      d = freec / total_capture;



		      // the result is all levels which are absorbed
		      // by the new dheight are gone
		      // the first level may be modified

		      // skip has the height until the new bunches

		      // some helpers
		      n_electrons_per_trap_express_ov = n_electrons_per_trap_express * ov;

                      if ( d < 1.0 )
                        {
			  #ifdef __debug
			  output( 10, "d < 1.0\n" );
			  #endif
			  // not easy needs a help array

			  new_nr_trapl = 0;

			  // use some helper pointer
			  dheight2 = dheight;
			  cheight2 = cheight;


			  // walk through all entries
			  h = 0;
			  for(j=nr_trapl-1;j>=0;j--)
			    {
			      if ( dheight2 < 0.0 )
				{
				  // nothing is needed anymore
				  // just copy that level
				  new_trapl[new_nr_trapl] = trapl[j];
				  new_trapl_fill[new_nr_trapl] = trapl_fill[j];
				  new_nr_trapl++;
				}
			      else
				{
				  // we need to modify existing levels
				  // scan each bunch of levels
				  if ( trapl_fill[j] < dheight2 )
				    {
				      // use the whole level
				      new_trapl[new_nr_trapl] = trapl[j] + ( n_electrons_per_trap_express - trapl[j] ) * d;
				      new_trapl_fill[new_nr_trapl] = trapl_fill[j];
				      new_nr_trapl++;
				    }
				  else
				    {
				      // the complex sitiuation
				      // the whole trap level needs to be split in 3 parts
				      // the first half filled level
				      // the 1 level for ov part
				      // the untouched part

				      #ifdef __debug
				      output( 10, "dheight2,trapl_fill[j],h: %f %i %i\n", dheight2, trapl_fill[j], h );
				      output( 10, "cheight2                : %i\n", cheight2 );
				      #endif

				      // idea, just judge, what to do with the number of available levels

				      if ( trapl_fill[j]  == 1 )
					{
					  // just one level to work on, easy
					  for (i=0;i<n_species;i++)
					    {
					      if ( trapl[j][i] < n_electrons_per_trap_express_ov[i] )
						{
						  new_trapl[new_nr_trapl][i] = trapl[j][i]
						    + (  n_electrons_per_trap_express_ov[i]  - trapl[j][i] ) * d;
						}
					      else
						new_trapl[new_nr_trapl][i] = trapl[j][i];
					    }
					  new_trapl_fill[new_nr_trapl] = 1;
					  new_nr_trapl++;
					}
				      else
					{
					  // the most default situation
					  // need to split in 3 parts
					  // larger modified, modified ov part, untouched part

					  if ( cheight2 > 0 )
					    {
					      // first part is necessary
					      new_trapl[new_nr_trapl] = trapl[j] + ( n_electrons_per_trap_express - trapl[j] ) * d;
					      new_trapl_fill[new_nr_trapl] = cheight2;
					      new_nr_trapl++;
					    }

					  if ( ov > 1e-14 )
					    {
					      // if ov == 0! then no additional level is needed
					      for (i=0;i<n_species;i++)
						if ( trapl[j][i] < n_electrons_per_trap_express_ov[i] )
						  new_trapl[new_nr_trapl][i] = trapl[j][i] + ( ( n_electrons_per_trap_express_ov[i]  ) - trapl[j][i] ) * d;
						else
						  new_trapl[new_nr_trapl][i] = trapl[j][i];
					      new_trapl_fill[new_nr_trapl] = 1;
					      new_nr_trapl++;
					    }


					  if ( cheight2 < trapl_fill[j] )
					    {
					      // add the last part if necessary
					      new_trapl[new_nr_trapl] = trapl[j];
					      new_trapl_fill[new_nr_trapl] = trapl_fill[j] - ceil( dheight2 );
					      new_nr_trapl++;
					    }
					}

				    }

				}
			      h += trapl_fill[j];
			      dheight2 -= trapl_fill[j];
			      cheight2 -= trapl_fill[j];
			    }

			  if ( dheight2 > 0.0 )
			    {
			      // we need a level
			      #ifdef __debug
			      output( 10, "new level needed: dheight2 = %f\n", dheight2 );
			      #endif
			      //int cheight2 = ceil( dheight2 ) -1 ;
			      if ( cheight2 > 0 )
				{
				  new_trapl[new_nr_trapl] = n_electrons_per_trap_express * d;
				  new_trapl_fill[new_nr_trapl] = cheight2;
				  new_nr_trapl++;
				}
			      new_trapl[new_nr_trapl] = n_electrons_per_trap_express_ov * d;
			      new_trapl_fill[new_nr_trapl] = 1;
			      new_nr_trapl++;
			    }

			  #ifdef __debug
			  output( 10, "Copying back the temporary data ..\n" );
			  #endif
			  i = 0;
			  for(j=new_nr_trapl-1;j>=0;j--)
			    {
			      trapl[i]      = new_trapl[j];
			      trapl_fill[i] = new_trapl_fill[j];
			      i++;
			    }
			  nr_trapl = new_nr_trapl;
			  total_capture *= d;
			}
                      else
                        {
                          // cheight full levels
			  // cheight+1 partly filled levels
			  //
			  // trapl_fill gives the number of filled levels


			  // prepare the stack for refilling

			  skip = 0;
			  h = 0;
			  while ( nr_trapl != 0 )
			    {
			      h += trapl_fill[nr_trapl-1];
			      if ( h > dheight )
				break;
			      else
				//skip += trapl_fill[nr_trapl-1];
				skip = h;
			      nr_trapl--;
			    }

			  #ifdef __debug
			  output( 10, "h,nr_trapl,d: %i %i %f\n", h, nr_trapl, dheight );
                          #endif

			  if ( skip == dheight )
			    {
			      // best solution ...

			      // but not correct implemented ...
			      #ifdef __debug
			      output( 10, "Balla balla\n" );
			      output( 10, "skip=%i dheight=%f\n", skip, dheight );
			      #endif
			      trapl_fill[nr_trapl+1] = cheight;
			      trapl_fill[nr_trapl] = 1;
			      trapl[nr_trapl+1] = n_electrons_per_trap_express;
			      trapl[nr_trapl] = n_electrons_per_trap_express_ov;
			      nr_trapl += 2;
			    }
			  else
			    {
			      // normal case

			      if ( nr_trapl == 0 )
				{
				  trapl_fill[1] = cheight;
				  trapl_fill[0] = 1;
				  trapl[1] = n_electrons_per_trap_express;
				  trapl[0] = n_electrons_per_trap_express_ov;
				  nr_trapl = 2;
				}
			      else
				{

				  // cheight full trap levels
				  // ov is a single level with ov * n_electrons_per_trap fill

				  // skip levels are already skipped

				  // modify the first levels
				  // using cheight because this is the number of remaining levels
				  // from the prior filled trap levels

				  #ifdef __debug
				  output( 10, "tr_f[0],ch,skip: %i %i %i\n", trapl_fill[0], cheight, skip );
				  #endif

				  trapl_fill[nr_trapl-1] -= cheight - skip;

				  if ( trapl_fill[nr_trapl-1] == 1 )
				    {
				      if ( val_array_smaller( trapl[nr_trapl-1], n_electrons_per_trap_express_ov ) )
					//if ( trapl[0].sum() < n_electrons_per_trap_express.sum() * ov )
					{
					  for (j=0;j<n_species;j++)
					    if ( trapl[nr_trapl-1][j] < n_electrons_per_trap_express_ov[j] )
					      trapl[nr_trapl-1][j] =  n_electrons_per_trap_express_ov[j];
					}
				    }
				  else
				    {
				      //print_trapl( trapl, trapl_fill, n_species, nr_trapl );
				      // check if we need an additional trap level
				      if ( val_array_smaller( trapl[nr_trapl-1], n_electrons_per_trap_express_ov ) )
					//if ( trapl[0].sum() < n_electrons_per_trap_express.sum() * ov )
					{
					  // yes, we need one
					  trapl_fill[nr_trapl-1]--;
					  trapl_fill[nr_trapl] = 1;
					  trapl[nr_trapl] = trapl[nr_trapl-1];
					  // fill the broken level up to the max
					  for (j=0;j<n_species;j++)
					    if ( trapl[nr_trapl][j] < n_electrons_per_trap_express_ov[j]  )
					      {
						trapl[nr_trapl][j] =  n_electrons_per_trap_express_ov[j];
					      }
					  nr_trapl++;
					}
				    }

				  // fill the leading trap level
				  trapl_fill[nr_trapl] = cheight;
				  trapl[nr_trapl] = n_electrons_per_trap_express;

				  nr_trapl ++;
				}
			    }
                        }

		      #ifdef __debug
		      print_trapl( trapl, trapl_fill, n_species, nr_trapl );
		      #endif

                      // delete the captured exlectrons from
                      //   the pixel value
                      freec -= total_capture;


		      // cleanup the array
		      // - removing/merge double levels ...
		      if ( dark_mode  == 1 )
		      	{
		      	  j = 0;
		      	  for (i=1;i<nr_trapl;i++)
		      	    {
                              #ifdef __debug
		      	      output( 10, "i,j: %i %i\n", i, j );
		      	      output( 10, "trapl[i,j] : %f %f\n", trapl[i].sum(), trapl[j].sum() );
		      	      output( 10, "diff       : %f\n", std::abs(  trapl[i].sum() - trapl[j].sum() ) );
		      	      #endif
		      	      if ( std::abs( trapl[i].sum() - trapl[j].sum() ) < 1e-11  )
		      		trapl_fill[j] += trapl_fill[i];
		      	      else
		      		{
		      		  j++;
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
		      print_trapl( trapl, trapl_fill, n_species, nr_trapl );
		      #endif

                    } /* end of if ( freec > well_notch_depth ) */

                  //#ifdef __debug
                  //if ( i_pixel == debug_pixel )
                  //  output( 1, "%i ", p_express_multiplier[i_pixel] );
                  //#endif
		  /* add the trail for i_pixel */

                  //(*image_data)[((i_pixel+start_y)*image_width)+i_column] += ( freec - im );

                  // save only the trail ...
                  trail[((i_pixel+start_y)*image_width)+i_column] = ( freec - im );

                } /* end of p_express_multiplier[i_pixel] != 0 */
            } /* i_pixel loop for the trail */
          p_express_multiplier += image_height +1;
        }  /* end of express loop */

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

      {
	double traps_total = 0.0;
	for (i=0;i<nr_trapl;i++)
	  traps_total += trapl[i].sum() * trapl_fill[i];
	output( 11, "%5i trap fill end: %f (%i)\n", i_column, traps_total, nr_trapl );
      }
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
  free( express_multiplier );
}



void cte_image_euclid::clock_charge_image_neo2( std::valarray<double> & image,
				    std::valarray<double> & trail,
				    std::valarray<long> & xrange,
				    std::valarray<long> & yrange )
{
  // local variables copied from parameter set
  // makes the code faster, since the access of the class pointer is
  // only once ...
  int     n_species;
  int     n_levels;
  double  well_depth;
  double  well_notch_depth;
  double  well_fill_power;
  double  well_range;
  int     express;
  int     readout_offset;

  /* CTE variables */
  double                traps_total;
  std::valarray<double> n_electrons_per_trap;
  std::valarray<double> n_electrons_per_trap_express;
  double                n_electrons_per_trap_total_express;
  int                  *express_multiplier;
  int                  *p_express_multiplier;
  int                   i_express;
  double                release;
  double                total_capture;
  double                freec;
  double                im;
  double                dheight;

  int                   sparse_pixels;

  int                   start_x, start_y, end_x, end_y;
  int                   i_column, i_pixel;


  /* helpers */
  int     i;
  double  d;
  double  sum;


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
      end_x = image_width;
    }
  else
    {
      start_x = xrange[0];
      end_x = xrange[1];
    }

  if ( yrange.size() < 2 )
    {
      start_y = 0;
      end_y = image_height;
    }
  else
    {
      start_y = yrange[0];
      end_y = yrange[1];
    }

  output( 10, "start_x=%i end_x=%i\n", start_x, end_x );
  output( 10, "start_y=%i end_y=%i\n", start_y, end_y );

  // special local variabels copied from parameter set
  // use n_species as a local variable
  n_species        = parameters->trap_density.size();
  n_levels         = parameters->n_levels;
  well_depth       = parameters->well_depth;
  well_notch_depth = parameters->well_notch_depth;
  well_fill_power  = parameters->well_fill_power;
  express          = parameters->express;
  readout_offset   = parameters->readout_offset;

  //traps_total = 0;
  //for (i=0;i<n_species;i++)
  //  traps_total += trap_density[i];
  traps_total = parameters->trap_density.sum();


  // if quantize_traps set
  // n_levels = round( 2.0 / traps_total );
  // if ( n_levels < 1  )
  //   n_levels = 1;


  // change the n_levels to a dynamic variable
  // n_levels = 10000;
  // (*pn_levels) = n_levels;

  // removed in favor of a class variable
  //n_levels = (*pn_levels);

  //n_electrons_per_trap_express = (double*) malloc( sizeof( double ) * n_species );

  //n_electrons_per_trap = (double*) malloc( sizeof( double ) * n_species );
  //for (i=0;i<n_species;i++)
  //    n_electrons_per_trap[i] = parameters->trap_density[i] / n_levels;
  n_electrons_per_trap = std::valarray<double> ( parameters->trap_density / (double) n_levels );
  //n_electrons_per_trap_total = traps_total / n_levels;

  n_electrons_per_trap_express = std::valarray<double> ( n_electrons_per_trap );

  sparse_pixels = get_sparse_pixels( image, traps_total );

  output( 1, "There are %i pixels containing more traps than charge.\nThe order in which these traps should be filled is ambiguous.\n", sparse_pixels );

  output( 1, "Using Jay Anderson's trick to speed up runtime\n" );
  output( 1, "Using Olli's idea to make the code faster and simpler! Warning, results may differ!\n" );


  output( 1, "Using n_levels=%i\n", n_levels );

  /* new code with variable express */

  express_multiplier = (int*) malloc( express * (image_height+1 ) * sizeof( int ) );

  for (i_pixel=0;i_pixel<image_height+1;i_pixel++)
    {
      int d;
      int d2;
      int i_sum = 0;
      for (i_express=0;i_express<express;i_express++)
        {
          int pos;
          d = ( i_pixel + 1 + readout_offset);
          d2= ((image_height+1+readout_offset)*(i_express+1))/express;
          if ( d > d2 )
            d = d2;

          d -= i_sum;
          i_sum += d;

          pos = (i_express*(image_height+1))+i_pixel;
          express_multiplier[pos] = d;
        }
     }


  // create the exponential_factor for the release calculations
  std::valarray<double> exponential_factor( 0.0, n_species );
  exponential_factor = 1 - exp( -1.0 / parameters->trap_lifetime );


  output( 10, "exponential_factor: %f %f %f\n", exponential_factor[0], exponential_factor[1], exponential_factor[2] );

  // well_range
  well_range = well_depth - well_notch_depth;



  // new trao definition
  // example trap, need for release time calculations
  // ntrap_fill is the number of levels (incl. fraction) of filled trap levels, the levels
  // are filled homogenous but not always to the maximum
  std::valarray<double> ntrap( 0.0, n_species );
  double ntrap_fill = 0.0;


  // initialize the time measurement
  gettimeofday( &start_time, NULL );
  getrusage( RUSAGE_SELF, &cpu_start_time );


  for (i_column=start_x;i_column<end_x;i_column++)
    {

      #ifdef __debug
      output( 1, "%i: ", i_column );
      #endif

      // use the multiplier as a flat array with the "pixel multiplicators"
      // in a line
      p_express_multiplier = express_multiplier;
      for (i_express=0;i_express<express;i_express++)
        {
	  // all traps are empty
	  ntrap = std::valarray<double> ( 0.0, n_species );
	  ntrap_fill = 0.0;




          for (i_pixel=0;i_pixel<(end_y-start_y);i_pixel++)
	    {

	      // inner pixel loop

              // check if we need to calculate a new trail for that pixel
              if ( p_express_multiplier[i_pixel] != 0 )
                {
                  // calculate the corrected n_electrons_per_trap for express
		  n_electrons_per_trap_express = n_electrons_per_trap *  (double) p_express_multiplier[i_pixel];
		  n_electrons_per_trap_total_express = n_electrons_per_trap_express.sum();

                  // extract pixel
		  im = image[((i_pixel+start_y)*image_width)+i_column];
                  if ( im > well_depth )
                    im = well_depth;
                  if ( im < 0.0 )
                    im = 0.0;
                  freec = im;

                  //#ifdef __debug
                  //if ( i_pixel == debug_pixel )
                  //  output( 1, "%f ", im );
                  //#endif


		  // Release any trapped electrons, using the appropriate decay half-life

                  // release a number of electrons in the traps
                  // trapped electrons relased exponentially

		  sum = 0.0;

		  // code checked if valarray is faster -> 50% slower ...
		  for (i=0;i<n_species;i++)
		    {
		      release = ntrap[i] * exponential_factor[i];
		      ntrap[i] -= release;
		      sum += release * ntrap_fill;
		    }


                  #ifdef __debug
                  if ( ( i_pixel == debug_pixel ) || ( i_pixel == debug_pixel+1 ) )
                    output( 1, "tid=%04i sum=%f\n", i_column, sum  );
                  #endif

		  // add released electrons to cthe pixel value
                  freec += sum;


                  // Capture any free electrons in the vicinity of empty traps

		  if ( freec > well_notch_depth )
                    {
                      d = pow(((freec - well_notch_depth ) / well_range ),well_fill_power);
                      if ( d > 1.0 )
                        d =  1.0;
                      else
                        if ( d < 0.0 )
                          d = 0.0;

                      dheight = n_levels * d;



		      // dheight is the electron cloud height

                      // calculate the number of electrons which can be captured



		      // new code


		      // the number of trap electrons which are not affected

		      std::valarray<double> trap_electron_rest( 0.0, n_species );

		      if ( dheight > ntrap_fill )
			{
			}
		      else
			trap_electron_rest = ntrap * ( ntrap_fill - dheight );
		      double trap_electron_rest_total = trap_electron_rest.sum();


		      // count all free places in which we can put the catched electrons
		      std::valarray<double>   max_capture( 0.0, n_species );

		      if ( dheight > ntrap_fill )
			{
			  max_capture  = ( n_electrons_per_trap_express - ntrap ) * ntrap_fill;
			  max_capture += n_electrons_per_trap_express * (  dheight - ntrap_fill );
			}
		      else
			{
			  double cheight = ceil( dheight );
			  double frac = dheight - cheight + 1;
			  max_capture = ( n_electrons_per_trap_express - ntrap ) * ( cheight - 1 );
			  for (i=0;i<n_species;i++)
			    {
			      double c = n_electrons_per_trap_express[i] * frac - ntrap[i];
			      if ( c > 0.0 )
				max_capture[i] += c;
			    }

			  // old code
			  //max_capture = ( n_electrons_per_trap_express - ntrap ) * dheight;
			}
		      double max_capture_total = max_capture.sum();

		      // look for the future capture potential which is not affected by the e- cloud

		      std::valarray<double> trap_pot_capture( 0.0, n_species );
		      if ( dheight > ntrap_fill )
			{
			}
		      else
			{
			  // possible electrons in all trap levels - all filled electrons - all possibly captured electrons
			  trap_pot_capture = n_electrons_per_trap_express * ( ceil( ntrap_fill ) - ceil( dheight) ) - ( ntrap * ( ntrap_fill - ceil( dheight) ) );
			}

		      double trap_pot_capture_total = trap_pot_capture.sum();

		      // this is the maximum of electrons which can be filled into the trap levels
		      total_capture = max_capture_total;

                      #ifdef __debg
		      output( 10, "debug:  %i\n", i_pixel );
		      output( 10, "ts,tc       : %f %f\n", ntrap.sum() * ( ceil( dheight ) - 1 ), ntrap.sum() * dheight );
		      output( 10, "ntrap       : %f %f %f * %f\n", ntrap[0], ntrap[1], ntrap[2], ntrap_fill );
		      output( 10, "ntrap_total : %f\n", ntrap.sum() * ntrap_fill );
		      output( 10, "free,dheight: %f %f\n", freec, dheight );
		      output( 10, "elect_rest  : %f %f %f = %f\n", trap_electron_rest[0], trap_electron_rest[1], trap_electron_rest[2], trap_electron_rest_total );
		      output( 10, "max_capture : %f %f %f = %f\n", max_capture[0], max_capture[1], max_capture[2], max_capture_total );
		      output( 10, "pot_capture : %f %f %f = %f\n", trap_pot_capture[0], trap_pot_capture[1], trap_pot_capture[2], trap_pot_capture_total );


                      #endif

                      if ( total_capture < 1e-14 )
                        total_capture = 1e-14;

		      d = freec / total_capture;   // percentage of new electrons to capture per trap level
                      if ( d < 1.0 )
                        {
                          // less electrons than required for capturing process
			  //output( 1, " freec < total_capture !!\n" );
			  total_capture = freec;
			  for (i=0;i<n_species;i++)
			    {
			      double c = ( n_electrons_per_trap_express[i] - ntrap[i] ) * d;
			      ntrap[i] += c;
			    }
			}
		      else
			{
			  // more electrons than required for capturing process
			  // -> fill all traps to the max

			  //for (i=0;i<n_species;i++)
			  //  {
			  //    ntrap[i] = n_electrons_per_trap_express[i];
			  //  }
			  ntrap = n_electrons_per_trap_express;
			}

		      ntrap_fill = dheight;
		      #ifdef __debug
		      output( 10, "ntrap(cap1) : %f %f %f * %f\n", ntrap[0], ntrap[1], ntrap[2], ntrap_fill );
		      #endif

		      // ... but, use the rest electrons and fill up the levels below ...

                      #ifdef __debug
		      if ( i_pixel == debug_pixel )
			{
			  //output( 1, "dheight=%f n_e_p_t_t_e=%f restadd=%f pc=%f\n", dheight, n_electrons_per_trap_total_express, trap_electron_rest / n_electrons_per_trap_total_express, trap_pot_capture_total );
			}
                      #endif
		      if ( trap_electron_rest_total  > 0.0 )
			{
			  // all electrons which needs to be distributed
			  double trap_max_fill = ntrap.sum();
			  double s = ntrap.sum() * dheight + trap_electron_rest_total;
			  // new code

			  // first calculate individual trap densities depending on
			  // the rest electrons and rhe missing capture capability
			  std::valarray<double> tot_electrons( 0.0, n_species );
			  tot_electrons = ntrap * dheight + trap_electron_rest;

			  // calculate the individual ntrap_fill value for each species
			  std::valarray<double> ntrap_fill_species( 0.0, n_species);
			  ntrap_fill_species = ( tot_electrons + trap_pot_capture ) / ntrap;

			  #ifdef __debug
			  output( 10, "fill_species: %f %f %f\n", ntrap_fill_species[0], ntrap_fill_species[1], ntrap_fill_species[2] );
			  #endif

			  // use the maximum of all values
			  double ntrap_fill3 = ntrap_fill_species.max();


			  // rebalance the ntrap values according to the max ntrap_fill3!
			  for (i=0;i<n_species;i++)
			    ntrap[i] *= 1.0 - ( trap_pot_capture[i] / ntrap_fill3 ) / ntrap[i];



			  ntrap_fill = ntrap_fill3;

			  ////double s = ntrap.sum() * dheight + trap_electron_rest_total;

			  // BIG question if we need here ntrap.sum() instead n_electrons_per_trap_total_express
			  // trap_max_fill=ntrap.sum() gives at most traps with trap_max_fill electrons which is
			  // better to save the behaviour of the newly filled levels

			  //double trap_max_fill = n_electrons_per_trap_total_express;  // maxbe wrong
			  ////double trap_max_fill = ntrap.sum();
			  double ntrap_fill2 = ( s + trap_pot_capture_total ) / trap_max_fill;

			  double f = 1 - ( ( trap_pot_capture_total / ntrap_fill2 ) / trap_max_fill );

                          #ifdef __debug
			  double t = ntrap.sum();
			  #endif

			  //for (i=0;i<n_species;i++)
			  //  ntrap[i] *= f;
			  #ifdef __debug
			  if ( i_pixel == debug_pixel )
			    {
			      output(1, "s=%f, ntrap_fill=%f ntrap_fill2=%f f=%f\n", s, ntrap_fill, ntrap_fill2, f );
			      //output(1, "ntrap_sum=%f tot1=%f tot2=%f\n", ntrap.sum()*f, ntrap.sum()*f*ntrap_fill2, t*dheight+trap_electron_rest );
			    }
			  #endif

			  //ntrap_fill = ntrap_fill2;

			  //// too simple assumtion
			  //double  d =  trap_electron_rest / n_electrons_per_trap_total_express;
			  //ntrap_fill += d;

			}

		      #ifdef __debug
		      output( 10, "ntrap       : %f %f %f * %f\n", ntrap[0], ntrap[1], ntrap[2], ntrap_fill );
		      output( 10, "ntrap_total : %f\n", ntrap.sum() * ntrap_fill );

		      std::valarray<double> pot_cap_check( 0.0, n_species );
		      pot_cap_check = ( n_electrons_per_trap_express - ntrap ) * ntrap_fill;
		      output( 10, "pot_cap_chk : %f %f %f = %f\n", pot_cap_check[0], pot_cap_check[1], pot_cap_check[2], pot_cap_check.sum() );
		      #endif

                      #ifdef __debug
		      if ( i_pixel == debug_pixel )
                        {
                          output( 1, "tc=%f\n", total_capture );
                      //    /*output( 1, "tca=%f ", get_sum_double_array( capture, cheight, n_species ) ); */
                        }
                      #endif



                      // delete the captured exlectrons from
                      //   the pixel value
                      freec -= total_capture;

                    } /* end of if ( freec > well_notch_depth ) */

                  //#ifdef __debug
		  //if ( i_pixel == debug_pixel )
                  //  output( 1, "%i ", p_express_multiplier[i_pixel] );
                  //#endif

                  /* add the trail for i_pixel */

		  // save only the trail ...
		  trail[((i_pixel+start_y)*image_width)+i_column] = ( freec - im );

                } /* end of p_express_multiplier[i_pixel] != 0 */
            } /* i_pixel loop for the trail */
          p_express_multiplier += image_height +1;
        }  /* end of express loop */

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
  free( express_multiplier );
}


void cte_image_euclid::clock_charge( std::shared_ptr<std::valarray<double>> im,
			      long w, long h,
			      std::valarray<long> & xrange,
			      std::valarray<long> & yrange,
			      bool unclock )
{
  output( 11, "cte_image_euclid::clock_charge\n" );

  image_width  = w;
  image_height = h;


  // make a local copy of the image data
  std::valarray<double> image( (*im) );

  std::valarray<double> trail( 0.0, image.size() );


  // check and liit the range variable
  if (  xrange.size() >= 2 )
    {
      if ( xrange[0] < 0 )
	xrange[0] = 0;
      if ( xrange[1] > image_width )
	xrange[1] = image_width;
    }
  if (  yrange.size() >= 2 )
    {
      if ( yrange[0] < 0 )
	yrange[0] = 0;
      if ( yrange[1] > image_height )
	yrange[1] = image_height;
    }


  if ( unclock )
    {
      for (unsigned int iteration=0;iteration<parameters->n_iterations;iteration++)
	{
	  output( 1, "Iteration %i/%i\n", iteration+1, parameters->n_iterations );

	  //if ( have_negative_values( model, naxes[0], naxes[1] ) == 1 )
          //  output( 1, "Image contains negative pixels!\n" );

          //model_readout = copy_image( (char*)model, naxes[0], naxes[1], DOUBLE_IMG );

          // do something
	  if (parameters->neo_algorithm2 == true )
	    clock_charge_image_neo2( image, trail, xrange, yrange );
	  else
	    {
	      if (parameters->neo_algorithm == true )
		clock_charge_image_neo( image, trail, xrange, yrange );
	      else
		clock_charge_image( image, trail, xrange, yrange );
	    }


	  // do some statistics after work
          output( 1, "Minmax(old image): %f %f\n", image.min(), image.max() );

          // sub the trail
	  image = (*im) - trail;

	  for (unsigned int i=0;i<image.size();i++)
	    trail[i] = 0.0;

	  // limit_to_max
          limit_to_max( image, (65536.-3) );

          output( 1, "Minmax(new image): %f %f\n", image.min(), image.max() );

	}
    }

  // copy the data back into the original image
  (*im) = image;
  //(*im) = (*trail);
}
