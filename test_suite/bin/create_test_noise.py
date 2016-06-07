#!/usr/bin/env python

# written by: Oliver Cordes 2016-05-30
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
mean     = 5.0
stddev   = 0.1


def syntax( exitcode=0 ):
    print( 'SYNTAX: %s' % sys.argv[0] )
    print( '  -h|-?           : this help page' )
    print( '  -d dir' )
    print( '  --test_dir=dir  : use this directory for output data ' )
    print( '')
    print( '  --mean=val       : use this mean value (default: 5.0)')
    print( '  --stddef=val     : use this stddev value (default: 0.1)')
    sys.exit( exitcode )

long_options = [ 'test_dir=', 'mean=', 'stddev=' ]

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
    elif ( key == '--mean' ):
        mean = val
    elif ( key == '--stddev' ):
        stddev = val




def preparations():
    if ( os.path.isdir( test_dir ) == False ):
        print( '%s is missing, creating ...' )
        print( 'Done.' )


def create_noise_file( width=1024,
                        height=1024,
                        mean = 5.0,
                        stddev = 0.3):
    #imdata = np.zeros((200, 100))   # height, width

    imdata = np.zeros( (height, width) )



    imdata = np.random.normal( mean, stddev, (height, width))

    hdu = fits.PrimaryHDU( imdata )

    fname = os.path.join( test_dir, 'noise_%ix%i.fits' % ( width, height ) )
    print( 'Saving %s ...' % fname )
    hdu.writeto( fname, clobber=True)


# main

print( 'Create test file...' )
create_noise_file( mean=mean, stddev=stddev )
print( 'Done.' )
