#
# acs_image
#
# written by: Oliver Cordes 2015-12-02
# changed by: Oliver Cordes 2016-02-29


import sys, os

import numpy as np 
from astropy.io import fits

from astropy.time import Time


from arctic_prg import arctic_prg


class ACS_image( object ):
    def __init__( self, filename, args, verbose=False, dark_mode=False, action='full' ):
        self._filename     = filename
        self._args         = args
        self._dark_mode    = dark_mode
        self._action       = action
        self._verbose      = verbose

        # filename parts
        self._basename     = ''
        self._prefix       = ''
        self._suffix       = ''

        # keywords
        self._date         = 0.0
        self._exptime      = 0.0

        # some reminders 
        self._AB_electrons = True
        self._CD_electrons = True


    def _timeobs2double( self, date_obs, time_obs ):
        s = date_obs + ' ' + time_obs
        t = Time( s, scale='utc' )    # scale parameter used for astropy version < 1.0 
        if ( self._verbose ):
            print( 'timeobs2double conversion:' )
            print( 's=%s jd=%f' % ( s, t.jd ) )
        return t.jd

    
    def _useafter2double( self, useafter ):
        dt = datetime.datetime.strptime( useafter, '%b %d %Y %H:%M:%S' )
        t = Time( dt )
        if ( self._verbose ):
            print( 'useafter2double conversion:' )
            print( 's=%s jd=%f' % ( useafter, t.jd ) )
        return t.jd

    
    def _split_image( self, data, header, amps ):

        # check image units
        if 'BUNIT' in header.keys():
            units = header['BUNIT'].upper()
        else:
            units = ''

        if ( 'ELECTRONS' not in units ):
            print( 'Warning: BUNIT of image must be ELECTRONS or ELECTRONS/S! Got \'%s\'!' % units )


        if ( units == 'ELECTRONS/S' ):
            # convert the data
            if ( amps == 'AB' ):
                self._AB_electrons = False
            else:
                self._CD_electrons = False
    
            data *= self._exptime
                

        # take the half
        split_x = int( data.shape[1] / 2 )


        new_hdr = fits.Header()
        new_hdr['DATE'] = self._date
        new_hdr['BUNIT'] = 'ELECTRONS'
        
        if ( amps == 'AB' ):
            data = data[::-1]
        
        # left image
        fname = self._basename + '_' + self._prefix + amps[0] + '.' + self._suffix

        left_image = data[:,0:split_x]

        left_header = new_hdr
        left_header['WFC_AMP'] = amps[0]
        print( '  Saving %s ...' % fname ),
        hdu = fits.PrimaryHDU( left_image, left_header )
        hdu.writeto( fname, clobber=True)
        print( 'Done.' )


        # right image
        fname = self._basename + '_' + self._prefix + amps[1] + '.' + self._suffix
        
        right_image = data[:,split_x:data.shape[1]]

        right_image  = right_image[:,::-1]

        right_header = new_hdr
        right_header['WFC_AMP'] = amps[1]
        print( '  Saving %s ...' % fname ),
        hdu = fits.PrimaryHDU( right_image, right_header )
        hdu.writeto( fname, clobber=True)
        print( 'Done.' )
        

    def _unsplit_image( self, amps, orig_header ):

        left_name  = self._basename + '_' + 'cte' + amps[0] + '.' + self._suffix
        right_name = self._basename + '_' + 'cte' + amps[1] + '.' + self._suffix

        print( '  Loading left image %s ...' % left_name ),
        left_image  = fits.open( left_name, 'readonly' )
        print( 'Done.' )

        print( '  Loading right image %s ...' % right_name ),
        right_image = fits.open( right_name, 'readonly' )
        print( 'Done.' )

        print( '  Reorganizing data (amplifier %s) ...' % amps ),
        left_data  = left_image[0].data
        right_data = right_image[0].data

        # reverse right image
        right_data = right_data[:,::-1]

        data = np.append( left_data, right_data, axis=1 )

        # flip if this is the upper image
        if ( amps == 'AB' ):
            data = data[::-1]


        # correct units
        if ( amps == 'AB' ):
            if ( self._AB_electrons == False ):
                data /= self._exptime
                
        if ( amps == 'CD' ):
            if ( self._CD_electrons == False ):
                data /= self._exptime

        
        #print data.shape

        
        header = left_image[0].header.copy()

        header = orig_header

        cards = left_image[0].header.cards 
        for i in cards:
            if ( i[0].find( 'CTE_') == 0 ):
                header.append( i )
                
        left_image.close()
        right_image.close()

        print( 'Done.' )
        return data, header
        

        
    
    def split( self ):

        print( ' Splitting ACS image ... ')
        image = fits.open( self._filename, 'readonly' )

        if ( self._verbose ):
            print( ' Image HDU list:' )
            image.info()


        # analyse the main header
        header = image[0].header
        keys   = header.keys()

        # get the observed date/time correctly
        if ( ( 'DATE-OBS' in keys ) and ( 'TIME-OBS' in keys ) ):
            self._date = self._timeobs2double( header['DATE-OBS'], header['TIME-OBS'] )
        else:
            print( 'No DATE-OBS/TIME-OBS information found in header! Trying USEAFTER keyword!' )
            if ( 'USEAFTER' in keys ):
                self._date = self._useafter2double( header['USEAFTER'] )
            else:
                print( 'No proper date/time information found in FITS headder!' )
                return 1

        # create the date after launch
        self._date -= 2452334.5     # launch date

            
        # get the exposure time correctly

        self._exptime =  header['EXPTIME']

        if ( self._dark_mode == True ):
            if ( 'MEANEXP' in keys ):
                self._exptime = header['MEANEXP']

                
        self._split_image( image[1].data, image[1].header, 'CD' )
        self._split_image( image[4].data, image[4].header, 'AB' )
        

        image.close()

        return 0


    def unsplit( self ):

        print( ' Recreation of the ACS image ...' )        
        ifimage = fits.open( self._filename, 'readonly' )

        image = fits.HDUList()
        # walk through all extensions
        for i in xrange( len(ifimage ) ):
            if ( i == 0 ):
                hdu = fits.PrimaryHDU( header = ifimage[i].header )
            elif ( i == 1 ):
                new_image, new_header = self._unsplit_image( 'CD', ifimage[i].header )
                hdu = fits.ImageHDU( data=new_image, header=new_header, name='SCI' )
            elif ( i == 4 ):
                new_image, new_header = self._unsplit_image( 'AB', ifimage[i].header )
                hdu = fits.ImageHDU( data=new_image, header=new_header, name='SCI' )
            else:
                hdu = fits.ImageHDU( data=ifimage[i].data, header=ifimage[i].header, name=ifimage[i].name )

            image.append( hdu )


        ofname = self._basename + '_' + 'cte' + '.' + self._suffix
        print( '  Saving ACS image %s ...' %ofname ),
        image.writeto( ofname, clobber=True, output_verify='warn' )

        ifimage.close()
        image.close()
        print( 'Done.' )
        
        #fits.info( self._filename )
        #fits.info( ofname )

        print( ' Done.' )
        
        return 0


    def correct( self ):           

        print( ' Correcting ACS sub images ...')
        prg = arctic_prg( self._args )
            
        for chip in 'ABCD':
            ifname = self._basename + '_' + self._prefix + chip + '.' + self._suffix
            ofname = self._basename + '_' + 'cte' + chip + '.' + self._suffix

            print( '  Correcting image %s ...' % ifname )
            
            #print ifname, ofname
            
            if ( os.access( ifname, os.R_OK ) ):
                pass
            else:
                print( 'WARNING: Can\'t access \'%s\'! Splitting was incorrect!' % ifname )
                return -1

            ret = prg.execute( ifname, ofname )

            if ( os.access( ofname, os.R_OK ) ):
                pass
            else:
                print( 'WARNING: Can\'t access \'%s\'! Correction was not successful!' % ofname )
                return -1

            print( '  Done.' )

        print( ' Done.' )
            
        return 0
    

    def run( self ):
        print( 'Working on %s ...' % self._filename )

        s = self._filename.split( '.' )
        t = '.'.join( s[:-1] ).split( '_' )
        self._basename = '_'.join( t[:-1] )
        self._prefix   = t[-1]
        self._suffix   = s[-1]

        if self._verbose:
            print( 'Filename splitting:' )
            print( '  basename = %s' %  self._basename)
            print( '  prefix   = %s' % self._prefix )
            print( '  suffix   = %s' %  self._suffix )

        if ( self._action == 'full' ):
            ret = self.split() 

            if ( ret == 0 ):
                ret = self.correct()

            if ( ret == 0 ):
                ret = self.unsplit()
        elif ( self._action == 'split' ):
             ret = self.split()
        elif ( self._action == 'unsplit' ):
            ret = self.unsplit()
        elif ( self._action == 'correct' ):
            ret = self.correct()
        else:
            print( 'Unknown action \'%s\'!' % self._action )
            ret = -1

        if ( ret != 0 ):
            print( 'Error occurred on image! Skip this image!' )
        
        print( 'Done.' )
    
