# arctic_prg.py
#
# written by: Oliver Cordes 2015-12-11
# changed by; Olicer Cordes 2016-02-22


import subprocess
import os

class arctic_prg( object ):
    def __init__( self, args ):
        self._args = args
        self._cmd  = 'arctic'

        self._check_cmd()


    def _check_which( self, apath ):
        if ( apath == '' ):
            cmd = 'which ' + self._cmd
        else:
            cmd = 'which ' + os.path.join( apath, self._cmd )
        #print cmd
        try:
            ret = subprocess.call( cmd, shell=True )
        except OSError as e:
            ret = -1

        return ret
    
        
    def _check_cmd( self ):
        ret = self._check_which( '' )
        if ( ret != 0 ):
            test_path = [ '.', '..', 'bin', '../bin' ]

            for i in test_path:
                ret = self._check_which( i )
                if ret == 0:
                    self._cmd = os.path.join( i, self._cmd )
                    return
        


    def execute( self, infile, outfile ):
        args = ' '.join( self._args )
        cmd = self._cmd + ' ' + args + ' ' + infile + ' ' + outfile
        ret = subprocess.call( cmd, shell=True )
        return ret 

        
