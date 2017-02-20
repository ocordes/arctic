#!/usr/bin/env python


# written by: Oliver Cordes 2015-12-02
# changed by: Oliver Cordes 2015-12-29


import os, sys

import getopt

from acs_image import *


verbose     = False
action      = 'full'
arctic_args = []

# check for parameters


def syntax( exitcode=0 ):
    sys.exit( exitcode )

if ( len( sys.argv ) < 1 ):
    syntax()

long_options = ['neo', 'action=']   # example [ 'h1=']

try:
    opts, args = getopt.gnu_getopt( sys.argv[1:], 'h?d:c:m:v', long_options )
except getopt.GetoptError as s:
    print( 'Error while parsing command parameters!' )
    syntax( 1 )


    
for key,val in opts:
    if ( key == '-?' ) or ( key == '-h' ):
        syntax()
    elif ( key == '-v' ):
        verbose = True
    elif ( key == '-d' ):
        arctic_args.append( '-d %s' % val )
    elif ( key == '-c' ):
        arctic_args.append( '-c %s' % val )
    elif ( key == '-m' ):
        arctic_args.append( '-m %s' % val )
    elif ( key == '--neo' ):
        arctic_args.append( '-neo' )
    elif ( key == '--action' ):
        action = val



# main

for fname in args:
    image = ACS_image( fname, arctic_args, verbose=verbose, action=action )
    image.run()

