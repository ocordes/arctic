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

/* cte_image_classic.hh

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2017-05-24

   $Id$

*/

#ifndef __cte_image_classic_h

#define __cte_image_classic_h 1


#include <valarray>
#include <memory>

#include <sys/time.h>

#include "cte_image.hh"
#include "params.hh"


class cte_image_classic : public cte_image {
public:
  cte_image_classic( void );
  cte_image_classic( std::shared_ptr<params> & );

  virtual void   clock_charge_setup( void );
  virtual void   clock_charge_clear( void );

  virtual void   clock_charge_save_traps( void );
  virtual void   clock_charge_restore_traps( void );
  virtual double clock_charge_pixel_release( void );
  virtual double clock_charge_pixel_total_capture( double, double );
  virtual void   clock_charge_pixel_capture_ov( double );
  virtual void   clock_charge_pixel_capture_full( void );
  virtual void   clock_charge_pixel_cleanup( void );
  virtual double clock_charge_trap_info( void );

private:
  #ifdef __debug
  void   print_traps( void );
  #endif

protected:
  // extern declarated variables
  bool                                 check_empty_traps;
  double                               empty_trap_limit;


  // trap definitions
  std::valarray<double>                traps;
  unsigned int                         n_levels_traps;
  unsigned int                         stat_count;
  std::valarray<double>                n_electrons_per_trap;
  double                               n_electrons_per_trap_total;
  std::valarray<double>                n_electrons_per_trap_express;
  double                               n_electrons_per_trap_express_total;
  std::valarray<double>                n_electrons_per_trap_express_ov;

  unsigned int                         cheight;
};

#endif
