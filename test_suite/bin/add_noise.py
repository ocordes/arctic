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


mean        = 5.0
stddev      = 0.1
lam         = 1.0
noise_model = 'normal'

infile      = None
outfile     = None


def syntax( exitcode=0 ):
    print( 'SYNTAX: %s' % sys.argv[0] )
    print( '  -h|-?           : this help page' )
    print( '  -i input_file   : use this input image' )
    print( '  -o output_file  : write this output image')
    print( '')
    print( '  --mean=val       : use this mean value (default: 5.0)')
    print( '  --stddef=val     : use this stddev value (default: 0.1)')
    print( '  --lambda=val     : use this lambda value (default: 1)')
    print( '  --model=val      : noise models (default: normal)')
    print( '                     normal  : gaussian distribution')
    print( '                     poisson : poisson distribuntion')
    print( '                     uniform : uniform distribution')
    sys.exit( exitcode )

long_options = [ 'mean=', 'stddev=', 'model=' ]

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
    elif ( key == '-o') :
        outfile = val
    elif ( key == '--mean' ):
        mean = float( val )
    elif ( key == '--stddev' ):
        stddev = float( val )
    elif ( key == '--model'):
        noise_model = val
    elif ( key == 'lambda' ):
        lam = float( val )


# chack parameter
if ( infile is None ):
    print( 'No infile given! Program aborted!')
    sys.exit( -1 )

if ( outfile is None ):
    print( 'No outfile given! Program aborted!' )
    sys.exit( -1 )

if ( noise_model not in [ 'normal', 'poisson', 'uniform' ]):
    print( 'Noise model uknown! Program aborted!')
    sys.exit( -1 )



def add_noise_to_file( infile, outfile ):
    inf = fits.open( infile, 'readonly' )

    imdata = inf[0].data

    if ( noise_model == 'normal'):
        print( 'using gaussion distribution')
        noise = np.random.normal( mean, stddev, imdata.shape )
    elif ( noise_model == 'poisson' ):
        print( 'using poisson distribution')
        noise = np.random.poisson( lam, imdata.shape )
    else:
        noise = np.zeros( imdata.shape )

    imdata += noise

    #hdu = fits.PrimaryHDU( imdata, header=inf[0].header )
    hdu = fits.PrimaryHDU( imdata )

    print( 'Saving %s ...' % outfile )
    hdu.writeto( outfile, clobber=True)


# main

print( 'Add noise to file...' )
add_noise_to_file( infile, outfile )
print( 'Done.' )
