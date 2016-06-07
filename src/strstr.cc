// strstr.cc

// written by: Oliver Cordes 2005
// changed by: Oliver Cordes 2015-06-22

/*
   Copyright (C) 2005 Oliver-Mark Cordes

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

// $Log$
// $Id: strstr.cc 844 2015-07-20 14:29:34Z ocordes $


#include <string>
#include <locale>  

//using namespace std;

#include "strstr.hh"


std::string std::CString::strtok ( bool  start, const std::string& delim  )
{
  std::string::size_type startpos, endpos;

  std::string retstr;

  if ( start == true )
    lastpos = 0;

  if ( lastpos != npos )   // noch nicht am Ende des strings ? <WB>
    {
      // nein, noch nicht ... <WB>
      startpos = find_first_not_of( delim, lastpos ); // ueberlese alle delim

      endpos = find_first_of( delim, startpos ); // suche 1. delim nach char

      lastpos = endpos;

      if ( endpos == npos )
	endpos = length();

      if ( startpos != npos )
	retstr.assign( *this, startpos, endpos-startpos );
    }


  return retstr;
}


std::string std::CString::clean_start( const char c )
{
  std::string retstr;

  unsigned int pos = 0;

  while ( (*this)[pos] == c ) pos++;

  retstr.assign( *this, pos, length() );

  return retstr;
}


std::string std::CString::clean_char( const char c )
{
  string retstr = (*this);

  std::string::size_type pos;


  while ( ( pos = retstr.find( c ) ) != retstr.npos )
    retstr.erase( pos, 1 );


  return retstr;
}



std::string std::CString::clean_white( void )
{
  return clean_char( ' ' );
}


std::string std::CString::toupper( void )
{
  string retstr = (*this);

  std::locale loc;
  
  for (std::string::size_type i=0; i<retstr.length(); ++i)
    retstr[i] = std::toupper(retstr[i],loc);

  return retstr;
}
