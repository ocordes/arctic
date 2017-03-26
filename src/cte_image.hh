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

/* cte_image.hh

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2017-03-26

   $Id$

*/

#ifndef __cte_image_h

#define __cte_image_h 1


#include <valarray>
#include <memory>

#include <sys/time.h>

#include "params.hh"

class cte_image {
public:
  cte_image( std::shared_ptr<params> );
  void     setup( long, long );
  virtual ~cte_image() {};
  void   clock_charge( std::shared_ptr<std::valarray<double> > );
  virtual void clock_charge_setup( void );
  virtual void clock_charge_clear( void );

  virtual void clock_charge_save_traps( void );
  virtual void clock_charge_restore_traps( void );
  virtual double clock_charge_pixel_release( void );
  virtual double clock_charge_pixel_total_capture( double, int );
  virtual void clock_charge_pixel_capture_ov( double );
  virtual void clock_charge_pixel_capture_full( void );
  virtual void   clock_charge_pixel_cleanup( void );
  virtual double clock_charge_trap_info( void );
//private:
  double get_difftime( struct timeval start, struct timeval end );
  long   get_sparse_pixels( std::valarray<double> &, double );
  void   limit_to_max( std::valarray<double> &, double );

  void   clock_charge_column( std::valarray<double> & image,
                                       long );
  void   clock_charge_image( std::valarray<double> & );

  double get_sum_double_array( double *array, int width, int height );


  void   create_express_multiplier( std::valarray<int> & express_multiplier,
                                    int h );
  void   create_exponential_factor( void );

protected:
  std::shared_ptr<params>              parameters;

  long                                 image_width;
  long                                 image_height;
  long                                 width;
  long                                 height;

  bool                                 rotate;
  bool                                 direction;
  std::image_slice                     is;

  // CTE local parameters
  int                                  n_species;
  int                                  n_levels;
  double                               well_depth;
  double                               well_notch_depth;
  double                               well_fill_power;
  double                               well_range;
  int                                  express;
  int                                  readout_offset;
  double                               traps_total;
  std::valarray<int>                   express_multiplier;
  std::valarray<double>                exponential_factor;
  long                                 start_x;
  long                                 end_x;
  long                                 start_y;
  long                                 end_y;
};



#endif
