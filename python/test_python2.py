#!/usr/bin/env python

import arctic
import numpy as np

from astropy.io import fits

# main

hdulist = fits.open('../demo/jbke01rkq_drkA.fits')

hdulist.info()

data   = hdulist[0].data
header = hdulist[0].header

date = hdulist[0].header['DATE']
date += +2452334.5

# fill the julian date into paramter class

print( data.shape )

# setup the CTE object
c = arctic.cte_image_neo()

# get the parameter class
p = c.parameters

# test the print command
print( 'express=%i' % p.express )

# change some special parameter
#p.express = 5
#p.n_iterations = 3
trap_density  = [ 0.293314, 0.780633, 0.658226 ]
trap_lifetime = [ 0.740000, 7.700000, 37.000000 ]

p.set_traps( trap_density, trap_lifetime )

a = p.get_trap_density( p.n_species )
print ( a )

# call setup is not necessary each time, exceptions are:
# change of express, readout_offset, and image rotation/readout_offset
# directions parameters
c.setup( data.shape[0], data.shape[1] )

adata = data.astype( np.float64 )

#c.clock_charge( adata )
bdata = adata.copy()
c.clock_charge2( adata, bdata )


hdu = fits.PrimaryHDU( bdata , header )
fname = 'testA.fits'
hdu.writeto( fname, clobber=True)
