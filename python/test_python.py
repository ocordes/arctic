#!/usr/bin/env python

import arctic
import numpy as np

from astropy.io import fits

# main
#a = np.array( [0,1,2], dtype='int32'  )

#arctic.test_function( a, b )

#print( a )


p = arctic.params_acs()
p.load_config( '../aifa/test_opt.cte' )



hdulist = fits.open('../demo/jbke01rkq_drkA.fits')

hdulist.info()

data   = hdulist[0].data
header = hdulist[0].header

date = hdulist[0].header['DATE']
date += +2452334.5

# fill the julian date into paramter class
p.calc_trap_config( date )


print( data.shape )

# setup the CTE object
c = arctic.cte_image_neo( p )
c.setup( data.shape[0], data.shape[1] )

# reshape the array
fdata = data.flatten( order='C')
afdata = fdata.astype( np.float64 )

c.clock_charge2( afdata )


odata =  afdata.reshape( data.shape[0], data.shape[1] )

hdu = fits.PrimaryHDU( odata , header )
fname = 'testA.fits'
hdu.writeto( fname, clobber=True)
