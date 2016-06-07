#!/usr/bin/env python

# written by: Oliver Cordes 2016-05-27
# changed by: Oliver Cordes 2016-05-30

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



test_dir = '../data'


def syntax( exitcode=0 ):
    print( 'SYNTAX: %s' % sys.argv[0] )
    print( '  -h|-?           : this help page' )
    print( '  -d dir' )
    print( '  --test_dir=dir  : use this directory for output data ' )
    print( '')
    sys.exit( exitcode )

long_options = [ 'test_dir=' ]

try:
    opts, args = getopt.gnu_getopt( sys.argv[1:], 'h?d:', long_options )
except getopt.GetoptError as s:
    print( 'Error while parsing command parameters!' )
    syntax( 1 )

for key, val in opts:
    if ( key == '-?' ) or ( key == '-h' ):
        syntax()
    elif ( key == '-d' ) or ( key == '--test_dir' ):
        test_dir = val




def preparations():
    if ( os.path.isdir( test_dir ) == False ):
        print( '%s is missing, creating ...' )
        print( 'Done.' )


def create_cross_file( width=1024,
                        height=1024,
                        cw=10, ch=10,
                        nw=10, nh=10,
                        val = 10000 ):
    #imdata = np.zeros((200, 100))   # height, width

    imdata = np.zeros( (height, width) )



    # create some things inside

    # create columns

    cols = float( width ) / nw
    rows = float( height ) / nh
    w = cw / 2
    h = ch / 2

    for i in range( nw ):
        start = int( cols * (i+0.5) - w )
        if ( start < 0 ): start = 0
        end   = int( cols * (i+0.5) + w )
        if ( end > width ): end = width

        for ii in range( start, end ):
            for j in range( height ):
                imdata[j,ii] = val

    for j in range( nh ):
        start = int( rows * (j+0.5) - h )
        if ( start < 0 ): start = 0
        end   = int( rows * (j+0.5) + h )
        if ( end > height ): end = height

        for jj in range( start, end ):
            for i in range( width ):
                imdata[jj,i] = val


    hdu = fits.PrimaryHDU( imdata )

    fname = os.path.join( test_dir, 'cross_%ix%i.fits' % ( width, height ) )
    print( 'Saving %s ...' % fname )
    hdu.writeto( fname, clobber=True)


# main

print( 'Create test file...' )
create_cross_file( nw=10, nh=10)
print( 'Done.' )
