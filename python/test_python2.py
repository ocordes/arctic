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
print c.parameters
trap_density  = [ 0.293314, 0.780633, 0.658226 ]
trap_lifetime = [ 0.740000, 7.700000, 37.000000 ]

c.set_traps( trap_density, trap_lifetime )

c.setup( data.shape[0], data.shape[1] )

adata = data.astype( np.float64 )

c.clock_charge( adata )


hdu = fits.PrimaryHDU( adata , header )
fname = 'testA.fits'
hdu.writeto( fname, clobber=True)
