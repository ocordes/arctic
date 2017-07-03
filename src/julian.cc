/* (C) Copyright 2013 by Oliver Cordes
        - ocordes ( at ) astro ( dot ) uni-bonn ( dot ) de


    This file is part of arctic-project

    arctic is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    arctic is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with arctic. If not, see <http://www.gnu.org/licenses/>.

*/

/* julian.cc

   written by: Oliver Cordes 2013-02-04
   changed by: Oliver Cordes 2017-05-15
   changed by: Ole Marggraf 2015-06-23
*/

#include <cmath>

/* I suppose this is taken from Numerical Recipies?
 * Seems there is a correction missing at large dates,
 * e.g. JD for 31.12.2035 is one day too low (compared to USNO calculator).
 *
 * Problem: leap year in 1980 is not accounted for...
 *
 * Solution: changed to different algorithm not using any offset
 *
 * - OM
 */

double juliandate( int year, int month, int day, int hour, int minute, int second )
{
  /*
SUBROUTINE JDATE(TAG,MONAT,JAHR,HR,MIN,SEC,JD)
      REAL*8 D,JD,TAG,HR,SEC
      D=HR/24.+MIN/1440.+SEC/86400.
      N=JAHR*365+(MONAT-1)*31+TAG
      IF(MONAT.GT.2) THEN
        JD=JAHR/4-INT(MONAT*.4+2.3)-INT((JAHR/100+1)*.75)+N+D
      ELSE
        JD=(JAHR-1)/4-INT((JAHR/100+1)*.75)+N+D
      ENDIF
      JD=JD-693960.5
      RETURN
      END
  */

  /*
   * old algorithm

  double D, N, JD;

  // take the 1.1.1980 as a reference
  year -= 1980;
  D = (hour / 24.0 ) + ( minute / 1440.0 ) + ( second / 86400. );
  N = year*365+(month-1)*31+day;
  if ( month > 2 )
    JD = (year / 4) - (int) ( month*.4+2.3) - (int) ( ( (year / 100 )+1 ) *0.75 ) + N + D;
  else
    JD = ( (year-1) / 4) - (int) ( ( (year / 100 )+1 ) *0.75 ) + N + D;
  JD += 2444239.5;
  */

  /*
   * new algorithm, taken from Fernie (1983), IAPPP 13, p16,
   * http://adsabs.harvard.edu/full/1983IAPPP..13...16F
   */

  double T, JD;

  T = hour/24. + minute/1440. + second/86400.;

  JD = 367*year - floor(7* (year + floor((month+9)/12.) )/4. )
                        - floor(3* ( floor( (year + floor((month-9)/7.) )/100. )+1 )/4. )
                        + floor(275.*month/9.) + day + T + 1721028.5;

  return JD;
}
