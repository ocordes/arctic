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

/* image_acs.hh

   written by: Oliver Cordes 2015-01-05
   changed by: Oliver Cordes 2016-07-04

   $Id: image_acs.hh 917 2015-12-02 10:00:30Z ocordes $

*/

#ifndef __image_acs_h

#define __image_acs_h 1


#include "image.hh"
#include "params.hh"
#include "params_acs.hh"

#include <CCfits/CCfits>

using namespace CCfits;

// Defines the simple FITS image class


class acs_image : public image{
public:
  acs_image( int argc, char *argv[] );
  virtual ~acs_image();
  virtual int clock_charge( void );
private:
  bool                                   sci_mode_dark;

  // special variables/reminders
  bool                                   electrons_per_sec;

  double  date2double( std::string & s );
};


#endif
