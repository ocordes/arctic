/* (C) Copyright 2010 by Oliver Cordes
        - ocordes ( at ) astro ( dot ) uni-bonn ( dot ) de


    This file is part of arctic.

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

/* output.h

   written by: Oliver Cordes 2010-07-20
   changed by: Oliver Cordes 2016-11-24

   $Id$

*/


#ifndef __output_h


#define __output_h 1



extern int debug_level;


void debug_init( int argc, char *argv[] );


bool is_debug( int );


void output( int, const char *, ... );

#endif
