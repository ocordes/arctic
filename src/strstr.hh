// strstr.hh

// written by: Oliver Cordes 2005
// changed by: Oliver Cordes 2017-05-16

/*
   Copyright (C) 2005-2017 Oliver-Mark Cordes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

*/

#ifndef __strstr_h
#define __strstr_h 1


#include <string>

namespace std
{
  class CString;
}

class std::CString : public std::string
{
  size_type  lastpos;
 public:
  CString() : string() { lastpos = 0; };
  CString( const char *s ) : string( s ) { lastpos = 0; };
  CString( const string & s, size_type pos = 0, size_type n = npos )
    : string( s, pos, n ) { lastpos = 0; };
  string strtok( bool  start, const string & delim );
  string clean_start( const char );
  string clean_char( const char );
  string clean_white( void );
  string toupper( void );
};


#endif
