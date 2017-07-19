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
   changed by: Oliver Cordes 2017-07-19
*/

#ifndef CTE_IMAGE_H

#define CTE_IMAGE_H 1


#include <valarray>
#include <memory>

#include <sys/time.h>

#include "params.hh"

// #define __debug 1

class cte_image
{
public:
  std::shared_ptr<params>              parameters;
  unsigned int                         image_width;
  unsigned int                         image_height;
  unsigned int                         width;
  unsigned int                         height;

  cte_image ( void );
  cte_image( std::shared_ptr<params> & );
  void     setup( unsigned int, unsigned int );
  virtual ~cte_image() {};
  void   clock_charge( std::valarray<double> & );
  virtual void clock_charge_setup( void );
  virtual void clock_charge_clear( void );

  virtual void clock_charge_save_traps( void );
  virtual void clock_charge_restore_traps( void );
  virtual double clock_charge_pixel_release( void );
  virtual double clock_charge_pixel_total_capture( double, double );
  virtual void clock_charge_pixel_capture_ov( double );
  virtual void clock_charge_pixel_capture_full( void );
  virtual void   clock_charge_pixel_cleanup( void );
  virtual double clock_charge_trap_info( void );
//private:
  double get_difftime( struct timeval start, struct timeval end );
  unsigned int get_sparse_pixels( std::valarray<double> &, double );
  void   limit_to_max( std::valarray<double> &, double );

  void   clock_charge_column( std::valarray<double> &, unsigned int );
  void   clock_charge_image( std::valarray<double> & );


  void   create_express_multiplier( std::valarray<unsigned int> & express_multiplier );
  void   create_exponential_factor( void );

protected:
  std::image_slice                     is;

  // CTE local parameters
  unsigned int                         n_species;
  double                               traps_total;
  std::valarray<unsigned int>          express_multiplier;
  std::valarray<double>                exponential_factor;
  unsigned int                         start_x;
  unsigned int                         end_x;
  unsigned int                         start_y;
  unsigned int                         end_y;
};

#endif
