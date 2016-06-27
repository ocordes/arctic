#!/usr/bin/env python

# written by: Oliver Cordes 2016-06-27
# changed by: Oliver Cordes 2016-06-27

import os, sys

import getopt


try:
    import numpy as np
except:
    print( 'Install the numpy python module!' )
    sys.exit( -1 )

try:
    from astropy.io import fits
except:
    print( 'Install the astropy python modules!' )
    sys.exit( -1 )


infile  = None
outfile = None
flip_x  = False
flip_y  = False
rotate  = False
arotate = False


def syntax( exitcode=0 ):
    print( 'SYNTAX: %s' % sys.argv[0] )
    print( '  -h|-?           : this help page' )
    print( '  -i infile       : input file ' )
    print( '  -o outfile      : output file')
    print( '  --flip_x        : flip along y axes')
    print( '  --flip_y        : flip along x axes')
    print( '  --rotate        : rotate 90 deg counter clockwise' )
    print( '  --arotate       : rotate 90 deg anti counter clockwise')
    sys.exit( exitcode )

long_options = [ 'flip_x', 'flip_y', 'rotate', 'arotate' ]

try:
    opts, args = getopt.gnu_getopt( sys.argv[1:], 'h?i:o:', long_options )
except getopt.GetoptError as s:
    print( 'Error while parsing command parameters!' )
    syntax( 1 )

for key, val in opts:
    if ( key == '-?' ) or ( key == '-h' ):
        syntax()
    elif ( key == '-i' ):
        infile = val
    elif ( key == '-o' ):
        outfile = val
    elif ( key == '--flip_x' ):
        flip_x = True
    elif ( key == '--flip_y' ):
        flip_y = True
    elif ( key == '--rotate' ):
        rotate = True
        atotate = False
    elif ( key == '--arotate' ):
        arotate = True
        rotate = False


# main

inf = fits.open( infile, 'readonly' )

imdata = inf[0].data


if ( rotate ):
    imdata = np.rot90( imdata )
if ( arotate ):
    imdata = np.rot90( imdata, 3 )
if ( flip_x ):
    imdata = np.fliplr( imdata )
if ( flip_y ):
    imdata = np.flipud( imdata )


hdu = fits.PrimaryHDU( imdata, header=inf[0].header )

print( 'Saving %s ...' % outfile )
hdu.writeto( outfile, clobber=True)

print( 'Done.' )
