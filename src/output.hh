/* (C) Copyright 2010 by Oliver Cordes
        - ocordes ( at ) astro ( dot ) uni-bonn ( dot ) de


    This file is part of animation.

    animation is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    animation is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with animation.  If not, see <http://www.gnu.org/licenses/>.

*/

/* output.h

   written by: Oliver Cordes 2010-07-20
   changed by: Oliver Cordes 2015-06-18

   $Id: output.hh 799 2015-06-18 14:59:30Z ocordes $

*/


#ifndef __output_h


#define __output_h 1



extern int debug_level;


void debug_init( int argc, char *argv[] );


void output( int, const char *, ... );

#endif
