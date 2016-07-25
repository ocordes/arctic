/* (C) Copyright 2013 by Oliver Cordes
        - ocordes ( at ) astro ( dot ) uni-bonn ( dot ) de


    This file is part of acs-cte-project.

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

/* cte_image.hh

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2016-07-25

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
  cte_image( std::shared_ptr<params> p);
  void   clock_charge( std::shared_ptr<std::valarray<double>>,
		       long, long,
		       std::valarray<long> &,
		       std::valarray<long> &,
		       bool );
private:
  double get_difftime( struct timeval start, struct timeval end );
  long   get_sparse_pixels( std::valarray<double> &, double );
  void   limit_to_max( std::valarray<double> &, double );
  double get_sum_double_array( double *array, int width, int height );
  void   print_traps( std::valarray<double> & t, int n_species, int trap_levels );
  void   print_trapl( std::valarray<std::valarray<double>> & trapl,
		      std::valarray<int> & trapl_fill,
		      int n_species,
		      int nr_trapl );
  bool   val_array_smaller( std::valarray<double> & v1,
			    std::valarray<double> & v2 );
  void   create_express_multiplier( std::valarray<int> & express_multiplier,
                                    int express,
                                    int h,
                                    int readout_offset );
  void   clock_charge_image( std::valarray<double> &,
			     std::valarray<long> &,
			     std::valarray<long> & );
  void   clock_charge_image_neo( std::valarray<double> &,
				 std::valarray<long> &,
				 std::valarray<long> & );
  void   clock_charge_image_neo2( std::valarray<double> &,
				  std::valarray<long> &,
				  std::valarray<long> & );
protected:
  std::shared_ptr<params>              parameters;

  long                                 image_width;
  long                                 image_height;
  long                                 width;
  long                                 height;

  bool                                 rotate;
  bool                                 direction;

  // extern declarated variables
  bool                                 check_empty_traps;
  double                               empty_trap_limit;
};



#endif
