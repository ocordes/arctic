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

/* cte_image_watermark.hh

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2017-04-27

   $Id$

*/

#ifndef __cte_image_watermark_h

#define __cte_image_watermark_h 1


#include <valarray>
#include <memory>

#include <sys/time.h>

#include "params.hh"

class cte_image_watermark : public cte_image {
public:
  cte_image_watermark( std::shared_ptr<params> p);

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
  void   print_wml( void );
protected:
  // extern declarated variables
  bool                                 check_empty_traps;
  double                               empty_trap_limit;
  double                               empty_trap_limit_neo2;
  double                               neo2_split_limit;

  int                                  dark_mode;

  // trap definitions
  std::valarray<std::valarray<double>> wml;
  std::valarray<double>               wml_fill;
  long                                nr_wml;

  std::valarray<std::valarray<double>> new_wml;
  std::valarray<double>               new_wml_fill;
  long                                new_nr_wml;

  std::valarray<std::valarray<double>> saved_wml;
  std::valarray<double>               saved_wml_fill;
  long                                saved_nr_wml;

  std::valarray<double>               trap_density;
  double                              trap_density_total;
  std::valarray<double>               trap_density_express;
  double                              trap_density_express_total;

  double                              cloud_height;
};



#endif
