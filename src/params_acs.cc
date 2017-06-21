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

/* params_acs.cc

   written by: Oliver Cordes 2015-06-18
   changed by: Oliver Cordes 2017-06-21

   $Id$

*/

#include <cstdlib>
#include <iostream>

#include "output.hh"
#include "params.hh"
#include "params_acs.hh"
#include "params_fits.hh"
#include "strstr.hh"


#define sqr(x) (x)*(x)


params_acs::params_acs() : params_fits()
{
  working_mode = "ACS";
}

void params_acs::parse_args( std::string & key, std::string & val, int & error )
{
  output( 11, "params_acs::parse_args( key=%s, val=%s)\n", key.c_str(), val.c_str() );

  params_fits::parse_args( key, val, error );
}


void params_acs::calc_trap_config( double date )
{
  double launch_date = 2452334.5;       /* Date of SM3B when ACS was installed */

  double temperature_date = 2453920.0;  /* Date of temperature decrement */
  double repair_date = 2454968.0;       /* Date of SM4 when ACS was repaired */

  double trap_initial_density_presm4 = 0.017845;
  double trap_growth_rate_presm4 = 3.5488e-4;


  //double trap_initial_density_postsm4 = 0.404611;
  //double trap_growth_rate_postsm4 = 2.93286e-4;

  // Updated for Massey et al. (2013)
  //double trap_initial_density_postsm4 = -0.159069  * 1.07;
  //double trap_growth_rate_postsm4     =  0.000491395 *1.07;


  // updated for Massey et al. (2013b)
  double trap_growth_rate_postsm4     = 0.00056512878;
  double trap_initial_density_postsm4 = -0.249304;

  double sm4_trap_release_time[3]   = { 0.74, 7.7, 37 };    /* pixels */

  #define n_sm4_trap_parameter 3

  //double sm4_trap_ratio[3]          = { 0.18, 0.61, 0.51 }; /* relative densities */
  double sm4_trap_ratio[3]          = { 0.169333, 0.450667, 0.380000 }; /* updated for Massey et al. (2013b) */
  double sm4_trap_ratio_sum;
  double sm4_temperature            = 273.15-81;            /* K */
  double k                          = 8.617343e-5;          /* eV/K */
  double DeltaE[3]                  = { 0.31, 0.34, 0.44 }; /* eV */

  double operating_temperature;


  n_species = n_sm4_trap_parameter;

  if ( date < temperature_date )
  {
    operating_temperature = 273.15-77;
  }
  else
  {
    operating_temperature = 273.15-81;
  }

  /*
  output( 1, "operating_temperature=%f\n", operating_temperature );
  */

  /* allocate memory for the traps */
  trap_density  = std::valarray<double>( 0.0, n_species );
  trap_lifetime = std::valarray<double>( 0.0, n_species );

  /* Trap release time propto exp(DeltaE/kT)/T^2 */
  for (unsigned int i=0;i<n_species;++i)
  {
    trap_lifetime[i] = sm4_trap_release_time[i] *
      sqr( operating_temperature / sm4_temperature ) *
      exp( DeltaE[i] / ( k * sm4_temperature*operating_temperature ) *
           ( operating_temperature - sm4_temperature ) );
  }

  /* Work out total trap densities at each observation */
  for (unsigned int i=0;i<n_species;++i)
  {
    if ( date < repair_date )
    {
      trap_density[i] = trap_initial_density_presm4 + trap_growth_rate_presm4 * ( date - launch_date );
    }
    else
    {
      trap_density[i] = trap_initial_density_postsm4 + trap_growth_rate_postsm4 * ( date - launch_date );
    }
  }

  /* Split traps between the species */
  sm4_trap_ratio_sum = 0.0;
  for (unsigned int i=0;i<n_species;++i)
  {
    sm4_trap_ratio_sum += sm4_trap_ratio[i];
  }

  for (unsigned int i=0;i<n_species;++i)
  {
    trap_density[i] *= (sm4_trap_ratio[i] / sm4_trap_ratio_sum );
  }


  output( 1, "Model has %f traps per pixel, %f days after launch.\n", trap_density.sum(), (date-launch_date) );

  //output( 1, "Model has %i trap species:\n", trap_density.size() );
  //output( 1, " Nr  density  lifetime\n" );
  //for (unsigned int j=0;j<trap_density.size();j++)
  //  output( 1, " %2i: %f %f\n", j+1, trap_density[j], trap_lifetime[j] );
}


void params_acs::check_params ( void )
{
}


void syntax_acs( void )
{
  std::cout << "    none" << std::endl;
}
