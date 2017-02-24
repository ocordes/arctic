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

/* cte_image_neo.hh

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2017-02-24

   $Id$

*/

#ifndef __cte_image_neo_h

#define __cte_image_neo_h 1


#include <valarray>
#include <memory>

#include <sys/time.h>

#include "cte_image.hh"
#include "params.hh"

class cte_image_neo : public cte_image {
public:
  cte_image_neo( std::shared_ptr<params> p);

  virtual void   clock_charge_setup( void );
  virtual void   clock_charge_clear( void );

  virtual void   clock_charge_save_traps( void );
  virtual void   clock_charge_restore_traps( void );
  virtual double clock_charge_pixel_release( void );
  virtual double clock_charge_pixel_total_capture( double, int );
  virtual void   clock_charge_pixel_capture_ov( double );
  virtual void   clock_charge_pixel_capture_full( void );
  virtual void   clock_charge_pixel_cleanup( void );
  virtual double clock_charge_trap_info( void );

private:
  void   print_trapl( std::valarray<std::valarray<double>> & trapl,
		      std::valarray<int> & trapl_fill,
		      int n_species,
		      int nr_trapl );
  bool   val_array_smaller( std::valarray<double> & v1,
			    std::valarray<double> & v2 );
protected:
  // extern declarated variables
  bool                                 check_empty_traps;
  double                               empty_trap_limit;
  double                               empty_trap_limit_neo2;

  int                                  dark_mode;

  // trap definitions
  std::valarray<std::valarray<double>> trapl;
  std::valarray<int>                   trapl_fill;
  long                                 nr_trapl;
  std::valarray<std::valarray<double>> new_trapl;
  std::valarray<int>                   new_trapl_fill;
  long                                 new_nr_trapl;
  std::valarray<std::valarray<double>> saved_trapl;
  std::valarray<int>                   saved_trapl_fill;
  long                                 saved_nr_trapl;

  std::valarray<double>                n_electrons_per_trap;
  double                               n_electrons_per_trap_total;
  std::valarray<double>                n_electrons_per_trap_express;
  double                               n_electrons_per_trap_express_total;
  std::valarray<double>                n_electrons_per_trap_express_ov;
  double                               dheight;
  unsigned int                         cheight;
  double                               ov;
};



#endif
